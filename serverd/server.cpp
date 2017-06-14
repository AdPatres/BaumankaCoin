#include "server.hpp"

#include "messages/version.hpp"

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
    {
      auto version = messages::create<messages::version>(msg.second);
      version.addr_from.ip = 
        peer_ptr->socket().remote_endpoint().address().to_v4().to_bytes();
      assert(version.addr_recv.port == m_acceptor.local_endpoint().port());

      peer_ptr->send(messages::verack());
      peer_ptr->send(messages::version(peer_ptr->socket().remote_endpoint(),
        m_acceptor.local_endpoint().port()));
      msg = peer_ptr->receive();
      assert(msg.first == "verack");

      msg = peer_ptr->receive();
      assert(msg.first == "getaddr");
      peer_ptr->send(m_make_addr());
      
      m_peers.push_front(version.addr_from);
    }
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
