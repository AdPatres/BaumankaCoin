#include "server.hpp"

#include "messages/getblocks.hpp"
#include "messages/version.hpp"
#include "messages/block.hpp"
#include <botan/hex.h>

#include <boost/bind.hpp>

#include <algorithm>
#include <exception>
#include <fstream>

namespace serverd
{
  std::unique_ptr<server> g_server_ptr;
}

using namespace serverd;
using namespace boost::asio::ip;

template<typename Value, class Container>
  bool
  find(const Container& c, const Value v)
  {
    for (auto el : c)
      if (el == v) return true;
    return false;
  }

server::server(uint16_t acc_port)
: m_acceptor(m_ios, tcp::endpoint(tcp::v4(), acc_port))
{ 
  // ask for other peers
  std::fstream fst("hosts");
  std::string ip, port;
  fst >> ip >> port;
  tcp::endpoint endp = *tcp::resolver(m_ios).resolve(
    tcp::resolver::query(tcp::v4(), ip, port));
  this->connect(endp, boost::bind(&server::m_handshake, this, _1, _2));
}

void
server::start()
{
  this->accept();
  m_ios.run();
}

void
server::stop()
{
  m_acceptor.cancel();
  m_acceptor.close();
  m_ios.stop();
}

void 
server::accept()
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
  this->accept();
  if (ec) throw std::runtime_error(ec.message());

  auto msg = peer_ptr->receive();
  if (msg.first == "version")
    m_handle_version(peer_ptr, messages::create<messages::version>(msg.second));
  else if (msg.first == "inv")
    m_handle_inv(peer_ptr, messages::create<messages::inv>(msg.second));
}
catch (std::exception& e)
{ std::cerr << e.what() << std::endl; }

void
server::connect(const tcp::endpoint& endp,
  server::ConnectHandler cb)
{
  std::cerr << "connecting " << endp.address().to_v4().to_string() << ':' << std::to_string(endp.port()) << std::endl;
  connection::pointer peer_ptr = connection::create(m_ios);
  peer_ptr->socket().async_connect(endp, boost::bind(cb,
    boost::asio::placeholders::error, peer_ptr));
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
      this->connect(tcp::endpoint(address_v4(addr.ip), addr.port), 
        boost::bind(&server::m_handshake, this, _1, _2));

  messages::getblocks gb;
  gb.hash = m_wallet.getLastBlockHash();
  peer_ptr->send(gb);
  msg = peer_ptr->receive();
  assert(msg.first == "inv");
  auto inv = messages::create<messages::inv>(msg.second);
  if (inv.inventory.size() == 1)
    {
      if (inv.inventory[0].type == messages::inv_vect::inv_type::error)
        {
          // error
        }
      else if (inv.inventory[0].type == messages::inv_vect::inv_type::msg_block)
        {
          std::cerr << std::to_string(inv.inventory[0].hash[0]) <<
            std::to_string(inv.inventory[0].hash[1]) <<
            std::to_string(inv.inventory[0].hash[2]) <<
            std::to_string(inv.inventory[0].hash[3]) << std::endl;
          uint32_t amount = messages::hash_to_32(inv.inventory[0].hash);
          std::cerr << "receiving " << std::to_string(amount) << " blocks" << std::endl;
          for (decltype(amount) i = 0; i < amount; ++i)
            {
              msg = peer_ptr->receive();
              assert(msg.first == "block");
            }
        }
    } 
}
catch (std::exception& e)
{ std::cerr << e.what() << std::endl; }

messages::addr
server::m_make_addr()
{
  messages::addr res;
  for (auto peer : m_peers)
    res.addr_list.push_back(peer);
  return std::move(res);
}

void
server::m_handle_inv(connection::pointer peer_ptr, const messages::inv& inv)
{
  messages::getdata getdata;
  for (const auto& iv : inv.inventory)
    if (iv.type != messages::inv_vect::inv_type::error)
      getdata.inventory.push_back(iv);
  peer_ptr->send(getdata);
  
  for (size_t i = 0; i < getdata.inventory.size(); ++i)
    {
      auto msg = peer_ptr->receive();
      if (msg.first == "block")
        {
          // TODO: block message
          // auto block = messages::create<messages::block>(msg.second);
          // TODO: check block  & make smth
        }
      else if (msg.first == "tx")
        {
          // TODO: same for txes
        }
    }
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
      std::cerr << "make inv empty" << std::endl;
      m_wallet.customize(7, m_wallet.getAddress());
      std::cerr << "size " << m_wallet.getBlockchainSize() << std::endl;
      inv.inventory.push_back(messages::inv_vect{
        messages::inv_vect::inv_type::msg_block, 
        messages::hash_from_32(m_wallet.getBlockchainSize())
      });
      for (const auto& block : m_wallet.getBlocksAfter(-1))
        block_pool.push_back(messages::block_message{ block.getBroadcastData() });
    }
  else 
    {
      std::cerr << "make inv from hash" << std::endl;
      auto block_id = m_wallet.findByHash(gb.hash);
      if (block_id > -1)
        inv.inventory.push_back(messages::inv_vect{
          messages::inv_vect::inv_type::msg_block, 
          messages::hash_from_32(static_cast<uint32_t>(
            m_wallet.getBlockchainSize() - block_id + 1)
        )});
      else
        inv.inventory.push_back(messages::inv_vect{
          messages::inv_vect::inv_type::error, m_wallet.getLastBlockHash()});
    }
  peer_ptr->send(inv);
  for (const auto& b : block_pool)  
    peer_ptr->send(b);
}
