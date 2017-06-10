#include "server.hpp"

#include "messages/version.hpp"

#include <boost/bind.hpp>

#include <exception>

namespace serverd
{
  std::unique_ptr<server> g_server_ptr;
}

using namespace serverd;
using namespace boost::asio::ip;

server::server(uint16_t port)
: m_acceptor(m_ios, tcp::endpoint(tcp::v4(), port))
{ }

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
    boost::bind(&server::handle_accept, this, 
      boost::asio::placeholders::error, new_conn_ptr));
}

void
server::handle_accept(const boost::system::error_code& error, 
  connection::pointer conn_ptr)
{
  this->accept();
  if (!error)
    {
      messages::version vermsg;
      messages::message ms("version", vermsg);
      conn_ptr->send(ms);
      std::cerr << "sended\n";
    }
}
