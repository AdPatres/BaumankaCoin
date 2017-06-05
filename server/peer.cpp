#include "peer.hpp"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/bind.hpp>

using namespace serverd;

peer::peer(boost::asio::io_service& ios)
: m_socket(ios)
{ }

void
peer::write(const std::string& msg)
{
  
  boost::asio::async_write(m_socket, 
                          boost::asio::buffer(msg), 
                          boost::bind(&peer::write_handler, this,
                                      boost::asio::placeholders::error, 
                                      boost::asio::placeholders::bytes_transferred));
}

void
peer::send_buffer(const boost::asio::streambuf& msg)
{
  m_socket.send(msg.data());
}

void
peer::write_handler(const boost::system::error_code& ec, 
                    std::size_t bytes_transferred)
{ }
