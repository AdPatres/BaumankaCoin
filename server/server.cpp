#include "server.hpp"

#include "message.hpp"

#include <streambuf>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace serverd;

server::server(unsigned short port)
: m_acceptor(m_ios, 
              boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 
                                            port))
{
  start_accept();
  m_ios.run();
};

server::~server()
{ this->stop(); }

void
server::start_accept()
{
    peer::pointer new_peer_ptr = peer::create(m_acceptor.get_io_service());
    if (!m_acceptor.is_open())
      m_acceptor.listen();
    m_acceptor.async_accept(new_peer_ptr->socket(), 
                            boost::bind(&server::handle_accept, this, 
                                        boost::asio::placeholders::error,
                                        new_peer_ptr));
}

void
server::handle_accept(const boost::system::error_code& error, 
                      peer::pointer peer_ptr)
{
  this->start_accept();
  if (!error)
    {
      net_addr addr(peer_ptr->socket().local_endpoint());
      boost::asio::streambuf buf;
      boost::archive::binary_oarchive arch(buf);
      arch & addr;
      peer_ptr->send_buffer(buf);
    }
}

void
server::stop()
{
  m_acceptor.cancel();
  m_acceptor.close();
  m_ios.stop();
}
