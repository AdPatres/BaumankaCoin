#pragma once

#include "message.hpp"

#include <memory> // shared_ptr

#include <boost/archive/binary_oarchive.hpp>
#include <boost/asio.hpp>

namespace serverd
{
  class connection // network connection repr
  {
  public:
    using pointer = std::shared_ptr<connection>;

    static pointer
    create(boost::asio::io_service& ios) 
    { return pointer(new connection(ios)); }   

    boost::asio::ip::tcp::socket&
    socket()
    { return m_sock; }

    void
    send(const messages::message& msg) // WTF only in header?
    {
      boost::asio::streambuf buf;
      boost::archive::binary_oarchive oa(buf);
      oa << msg;
      m_sock.send(buf.data());
      std::cerr << "sended\n";
    }

  private:
    connection(boost::asio::io_service&);

    boost::asio::ip::tcp::socket m_sock;
  };

} // namespace serverd
