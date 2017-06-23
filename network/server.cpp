#include "server.hpp"

#include "messages/getblocks.hpp"
#include "messages/version.hpp"
#include "messages/block.hpp"
#include "messages/tx.hpp"

#include <fstream>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

using namespace boost::asio::ip;
namespace pt = boost::property_tree;
using namespace serverd;

template<typename Value, class Container>
  bool
  find(const Container& c, const Value v)
  {
    for (auto el : c)
      if (el == v) return true;
    return false;
  }

server::server()
: m_acceptor(tcp::acceptor(m_ios))
{
  m_load();
  std::cout << m_port << std::endl;
  auto local_endp = tcp::endpoint(tcp::v4(), m_port);
  m_acceptor.open(local_endp.protocol());
  m_acceptor.bind(local_endp);

  // std::fstream fst("hosts");
  // std::string ip, port;
  // fst >> ip >> port;
  // tcp::endpoint endp = *tcp::resolver(m_ios).resolve(
  //   tcp::resolver::query(tcp::v4(), ip, port));
  // this->m_connect(endp, boost::bind(&server::m_handshake, this, _1, _2));
}

server::~server()
{ this->stop(); }

void 
server::m_load(const std::string& filename)
{
  pt::ptree tree;
  pt::read_info(filename, tree);
  m_port = tree.get("port", 8333);
}

void
server::start()
{
  m_acc_th_ptr.reset(new std::thread(&server::m_run, this));
}

void
server::stop()
{ 
  m_ios.stop();
  if (!m_acceptor.is_open())
    return;
  m_acceptor.cancel();
  m_acceptor.close();
  if (m_acc_th_ptr.get())
    m_acc_th_ptr->join();
}

void
server::m_run()
{
  m_ios.run();
  m_accept();
}

void
server::m_accept()
{
  connection::pointer new_conn_ptr = connection::create(m_ios);
  if (!m_acceptor.is_open())
    m_acceptor.listen();
  m_acceptor.async_accept(new_conn_ptr->socket(), 
    boost::bind(&server::m_handle_accept, this, 
      boost::asio::placeholders::error, new_conn_ptr));
}

void
server::m_handle_accept(const boost::system::error_code& ec, 
  connection::pointer peer_ptr)
try
{
  m_accept();
  if (ec) throw std::runtime_error(ec.message());

  auto msg = peer_ptr->receive();
  if (msg.first == "version") {}
    // m_handle_version(peer_ptr, messages::create<messages::version>(msg.second));
  else if (msg.first == "inv") {}
    // m_handle_inv(peer_ptr, messages::create<messages::inv>(msg.second));
}
catch (std::exception& e)
{ std::cerr << e.what() << std::endl; }

void
server::m_connect(const tcp::endpoint& endp, ConnectHandler cb)
{
  connection::pointer peer_ptr = connection::create(m_ios);
  peer_ptr->socket().async_connect(endp, boost::bind(cb,
    boost::asio::placeholders::error, peer_ptr));
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
server::m_handle_version(connection::pointer peer_ptr, 
  auto version)
{
  version.addr_from.ip = 
    peer_ptr->socket().remote_endpoint().address().to_v4().to_bytes();
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
      // m_blockchain_ptr->customize(7, m_blockchain_ptr->getAddress());
      inv.inventory.push_back(messages::inv_vect{
        messages::inv_vect::inv_type::msg_block, 
        messages::hash_from_32(m_blockchain_ptr->getBlockchainSize())
      });
      for (const auto& block : m_blockchain_ptr->getBlocksAfter(-1))
        block_pool.push_back(messages::block_message{block.getBroadcastData()});
    }
  else 
    {
      auto block_id = m_blockchain_ptr->findByHash(gb.hash);
      if (block_id > -1)
        inv.inventory.push_back(messages::inv_vect{
          messages::inv_vect::inv_type::msg_block, 
          messages::hash_from_32(m_blockchain_ptr->getBlockchainSize() - block_id + 1)
        });
      else
        inv.inventory.push_back(messages::inv_vect{
          messages::inv_vect::inv_type::error, m_blockchain_ptr->getLastBlockHash()});
    }
  peer_ptr->send(inv);
  for (const auto& b : block_pool)  
    peer_ptr->send(b);
}

void
server::m_handle_inv(connection::pointer peer_ptr, const messages::inv& inv)
{
  // for (const auto& row : inv.inventory)
  //   {
  //     uint32_t amount = messages::hash_to_32(row.hash);
  //     if (row.type == messages::inv_vect::inv_type::msg_block)
  //       {
  //         for (uint32_t i = 0; i < amount; ++i)
  //           {
  //             auto msg = peer_ptr->receive();
  //             assert(msg.first == "block");
  //             auto block_msg = messages::create<messages::block_message>(msg.second);
  //             Block block;
  //             uint32_t position = 0;
  //             block.scanBroadcastedData(block_msg.data, position);
  //             m_blockchain_ptr->addBlock(block);
  //           }
  //       }
  //     else if (row.type == messages::inv_vect::inv_type::msg_tx)
  //       {
  //         for (uint32_t i = 0; i < amount; ++i)
  //           {
  //             auto msg = peer_ptr->receive();
  //             assert(msg.first == "tx");
  //             auto tx_msg = messages::create<messages::tx>(msg.second);
  //             Transaction tx;
  //             uint32_t position = 0;
  //             tx.scanBroadcastedData(tx_msg.data, position);
  //             m_blockchain_ptr->addTx(tx);
  //           }
  //       }
  //   }
}

void
server::m_handshake(const boost::system::error_code& ec, 
  connection::pointer peer_ptr)
try
{
  if (ec) throw std::runtime_error(ec.message());

  peer_ptr->send(messages::version(peer_ptr->socket().remote_endpoint(), 
    m_acceptor.local_endpoint().port()));
  auto msg = peer_ptr->receive();
  assert(msg.first == "verack");

  msg = peer_ptr->receive();
  assert(msg.first == "version");
  auto version = messages::create<messages::version>(msg.second);
  assert(version.addr_recv.port == peer_ptr->socket().local_endpoint().port());
  peer_ptr->send(messages::verack());
  version.addr_from.ip = 
    peer_ptr->socket().remote_endpoint().address().to_v4().to_bytes();
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
      else if (inv.inventory[0].type == messages::inv_vect::inv_type::msg_block)
        {
          uint32_t amount = messages::hash_to_32(inv.inventory[0].hash);
          for (decltype(amount) i = 0; i < amount; ++i)
            {
              msg = peer_ptr->receive();
              assert(msg.first == "block");
              auto block_msg = messages::create<messages::block_message>(msg.second);
              Block block;
              uint32_t position = 0;
              block.scanBroadcastedData(block_msg.data, position);
              m_blockchain_ptr->addBlock(block);
            }
        }
    } 
}
catch (std::exception& e)
{ std::cerr << e.what() << std::endl; }
