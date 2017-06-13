#include "server.hpp"

#include "messages/version.hpp"

#include <boost/bind.hpp>

#include <exception>
#include <fstream>

namespace serverd
{
  std::unique_ptr<server> g_server_ptr;
}

using namespace serverd;
using namespace boost::asio::ip;

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
    }
}
catch (std::exception& e)
{ std::cerr << e.what() << std::endl; }

void
server::connect(const boost::asio::ip::tcp::endpoint& endp,
  server::ConnectHandler cb)
{
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
}
catch (std::exception& e)
{ std::cerr << e.what() << std::endl; }
