#include "./server.hpp"

#include "./messages/block.hpp"
#include "./messages/getblocks.hpp"
#include "./messages/tx.hpp"
#include "./messages/version.hpp"

#include <fstream>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <botan/sha2_32.h>

using namespace ad_patres;
using namespace boost::asio::ip;
using namespace Botan;
namespace pt = boost::property_tree;

template <typename Value, class Container>
bool
find(const Container& c, const Value v)
{
  for (auto el : c)
    if (el == v)
      return true;
  return false;
}

server::server() : m_acceptor(tcp::acceptor(m_ios))
{
  pt::ptree tree;
  pt::read_info("adpatres.conf", tree);
  m_port = tree.get("port", 8333);
  auto local_endp = tcp::endpoint(tcp::v4(), m_port);
  m_acceptor.open(local_endp.protocol());
  m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  m_acceptor.bind(local_endp);
  std::cout << "listening at port " << m_port << std::endl;

  BOOST_FOREACH (pt::ptree::value_type& v, tree.get_child("hosts"))
    {
      auto ip = v.second.get("ip", "0.0.0.0");
      auto port = v.second.get("port", "8333");
      if (ip != "0.0.0.0")
        {
          tcp::endpoint endp = *tcp::resolver(m_ios).resolve(
            tcp::resolver::query(tcp::v4(), ip, port));
          this->m_connect(endp,
                          boost::bind(&server::m_handshake, this, _1, _2));
        }
    }
}

server::~server() { this->stop(); }
void
server::start()
{
  m_acc_th_ptr.reset(new std::thread(&server::m_run, this));
}

void
server::stop()
{
  m_ios.stop();
  m_acceptor.cancel();
  if (!m_acceptor.is_open())
    return;
  m_acceptor.close();
  if (m_acc_th_ptr.get())
    m_acc_th_ptr->join();
}

void
server::m_run()
{
  m_acceptor.listen();
  m_accept();
  m_ios.run();
}

void
server::m_accept()
{
  connection::pointer new_conn_ptr = connection::create(m_ios);
  if (!m_acceptor.is_open())
    m_acceptor.listen();
  m_acceptor.async_accept(new_conn_ptr->socket(),
                          boost::bind(&server::m_handle_accept, this,
                                      boost::asio::placeholders::error,
                                      new_conn_ptr));
}

void
server::m_handle_accept(const boost::system::error_code& ec,
                        connection::pointer peer_ptr) try
  {
    m_accept();
    if (ec)
      throw std::runtime_error(ec.message());

    std::cerr
      << "accepted from "
      << peer_ptr->socket().remote_endpoint().address().to_v4().to_string()
      << std::endl;
    auto msg = peer_ptr->receive();
    if (msg.first == "version")
      m_handle_version(peer_ptr,
                       messages::create<messages::version>(msg.second));
    else if (msg.first == "inv")
      m_handle_inv(peer_ptr, messages::create<messages::inv>(msg.second));
  }
catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

void
server::m_connect(const tcp::endpoint& endp, ConnectHandler cb)
{
  connection::pointer peer_ptr = connection::create(m_ios);
  std::cerr << "connecting to " << endp.address().to_v4().to_string() << ':'
            << std::to_string(endp.port()) << std::endl;
  peer_ptr->socket().async_connect(
    endp, boost::bind(cb, boost::asio::placeholders::error, peer_ptr));
}

messages::addr
server::m_make_addr()
{
  messages::addr res;
  for (auto peer : m_peers)
    res.addr_list.push_back(peer);
  return std::move(res);
}

void
server::m_handle_version(connection::pointer peer_ptr, auto version)
{
  version.addr_from.ip
    = peer_ptr->socket().remote_endpoint().address().to_v4().to_bytes();
  assert(version.addr_recv.port == m_acceptor.local_endpoint().port());

  peer_ptr->send(messages::verack());
  peer_ptr->send(messages::version(peer_ptr->socket().remote_endpoint(),
                                   m_acceptor.local_endpoint().port()));
  auto msg = peer_ptr->receive();
  assert(msg.first == "verack");

  msg = peer_ptr->receive();
  assert(msg.first == "getaddr");
  peer_ptr->send(m_make_addr());
  m_peers.push_front(version.addr_from);

  msg = peer_ptr->receive();
  assert(msg.first == "getblocks");
  auto gb = messages::create<messages::getblocks>(msg.second);

  messages::inv inv;
  std::vector<messages::block_message> block_pool;
  if (gb.hash == SHA_256().process(Block().getBlockData()))
    {
      m_blockchain_ptr->customize(7, secure_vector<byte>(32, 0));
      inv.inventory.push_back(messages::inv_vect{
        messages::inv_vect::inv_type::msg_block,
        messages::hash_from_32(m_blockchain_ptr->getBlockchainSize())});
      for (const auto& block : m_blockchain_ptr->getBlocksAfter(-1))
        block_pool.push_back(messages::block_message{block.getBroadcastData()});
    }
  else
    {
      auto block_id = m_blockchain_ptr->findByHash(gb.hash);
      if (block_id > -1)
        inv.inventory.push_back(messages::inv_vect{
          messages::inv_vect::inv_type::msg_block,
          messages::hash_from_32(m_blockchain_ptr->getBlockchainSize()
                                 - block_id + 1)});
      else
        inv.inventory.push_back(
          messages::inv_vect{messages::inv_vect::inv_type::error,
                             m_blockchain_ptr->getLastBlockHash()});
    }
  peer_ptr->send(inv);
  for (const auto& b : block_pool)
    peer_ptr->send(b);
}

void
server::m_handle_inv(connection::pointer peer_ptr, const messages::inv& inv)
{
  for (const auto& row : inv.inventory)
    {
      uint32_t amount = messages::hash_to_32(row.hash);
      if (row.type == messages::inv_vect::inv_type::msg_block)
        {
          for (uint32_t i = 0; i < amount; ++i)
            {
              auto msg = peer_ptr->receive();
              assert(msg.first == "block");
              auto block_msg
                = messages::create<messages::block_message>(msg.second);
              Block block;
              uint32_t position = 0;
              block.scanBroadcastedData(block_msg.data, position);
              block.showInfo();
              m_blockchain_ptr->addBlock(block);
            }
        }
      else if (row.type == messages::inv_vect::inv_type::msg_tx)
        {
          for (uint32_t i = 0; i < amount; ++i)
            {
              auto msg = peer_ptr->receive();
              assert(msg.first == "tx");
              auto tx_msg = messages::create<messages::tx>(msg.second);
              Transaction tx;
              uint32_t position = 0;
              tx.scanBroadcastedData(tx_msg.data, position);
              tx.showInfo();
              m_blockchain_ptr->addTx(tx);
            }
        }
    }
}

void
server::m_handshake(const boost::system::error_code& ec,
                    connection::pointer peer_ptr) try
  {
    if (ec)
      throw std::runtime_error(ec.message());

    peer_ptr->send(messages::version(peer_ptr->socket().remote_endpoint(),
                                     m_acceptor.local_endpoint().port()));
    auto msg = peer_ptr->receive();
    assert(msg.first == "verack");

    msg = peer_ptr->receive();
    assert(msg.first == "version");
    auto version = messages::create<messages::version>(msg.second);
    assert(version.addr_recv.port
           == peer_ptr->socket().local_endpoint().port());
    peer_ptr->send(messages::verack());
    version.addr_from.ip
      = peer_ptr->socket().remote_endpoint().address().to_v4().to_bytes();
    m_peers.push_front(version.addr_from);

    peer_ptr->send(messages::getaddr());
    msg = peer_ptr->receive();
    assert(msg.first == "addr");
    auto addr_list = messages::create<messages::addr>(msg.second);
    for (auto addr : addr_list.addr_list)
      if (!find(m_peers, addr))
        m_connect(tcp::endpoint(address_v4(addr.ip), addr.port),
                  boost::bind(&server::m_handshake, this, _1, _2));

    messages::getblocks gb;
    gb.hash = m_blockchain_ptr->getLastBlockHash();
    peer_ptr->send(gb);
    msg = peer_ptr->receive();
    assert(msg.first == "inv");
    auto inv = messages::create<messages::inv>(msg.second);
    if (inv.inventory.size() == 1)
      {
        if (inv.inventory[0].type == messages::inv_vect::inv_type::error)
          {
            // TODO: error handle
          }
        else if (inv.inventory[0].type
                 == messages::inv_vect::inv_type::msg_block)
          {
            uint32_t amount = messages::hash_to_32(inv.inventory[0].hash);
            for (decltype(amount) i = 0; i < amount; ++i)
              {
                msg = peer_ptr->receive();
                assert(msg.first == "block");
                auto block_msg
                  = messages::create<messages::block_message>(msg.second);
                Block block;
                uint32_t position = 0;
                block.scanBroadcastedData(block_msg.data, position);
                block.showInfo();
                m_blockchain_ptr->addBlock(block);
              }
          }
      }
  }
catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

// void
// server::share(auto data)
// {
//   for (const auto& addr : m_peers)
//     m_connect(tcp::endpoint(address_v4(addr.ip), addr.port),
//       boost::bind(&server::m_share_handler, this, _1, _2, data));
// }

void
server::share(const Block& b)
{
  for (const auto& addr : m_peers)
    m_connect(tcp::endpoint(address_v4(addr.ip), addr.port),
              [this, b](const boost::system::error_code&,
                        connection::pointer peer_ptr) {
                messages::inv inv;
                inv.inventory.push_back(
                  messages::inv_vect{messages::inv_vect::inv_type::msg_block,
                                     messages::hash_from_32(1)});
                peer_ptr->send(inv);
                messages::block_message block_msg{b.getBroadcastData()};
                peer_ptr->send(block_msg);
                b.showInfo();
              });
}

void
server::share(const Transaction& tx)
{
  for (const auto& addr : m_peers)
    m_connect(tcp::endpoint(address_v4(addr.ip), addr.port),
              [this, tx](const boost::system::error_code&,
                         connection::pointer peer_ptr) {
                messages::inv inv;
                inv.inventory.push_back(
                  messages::inv_vect{messages::inv_vect::inv_type::msg_tx,
                                     messages::hash_from_32(1)});
                peer_ptr->send(inv);
                messages::tx tx_msg{tx.getBroadcastData()};
                peer_ptr->send(tx_msg);
                tx.showInfo();
              });
}

// template<>
//   void
//   server::m_share_handler<Transaction>(const boost::system::error_code&,
//     connection::pointer peer_ptr, Transaction& tx)
//   {
//     messages::inv inv;
//     inv.inventory.push_back(messages::inv_vect{
//       messages::inv_vect::inv_type::msg_tx, messages::hash_from_32(1) });
//     peer_ptr->send(inv);
//     messages::tx tx_msg{ tx.getBroadcastData() };
//     peer_ptr->send(tx_msg);
//   }

// template<>
//   void
//   server::m_share_handler<Block>(Block& b,
//     connection::pointer peer_ptr)
//   {
//     messages::inv inv;
//     inv.inventory.push_back(messages::inv_vect{
//       messages::inv_vect::inv_type::msg_block, messages::hash_from_32(1) });
//     peer_ptr->send(inv);
//     messages::block_message block_msg{ b.getBroadcastData() };
//     peer_ptr->send(block_msg);
//   }
