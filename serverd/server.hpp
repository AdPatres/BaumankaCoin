#pragma once

#include "connection.hpp"

#include <cstdint> // uint16_t
#include <memory> // unique_ptr
#include <iostream> 

#include <boost/asio.hpp> 

namespace serverd
{
  class server
  {
  public:
    explicit
    server(uint16_t);

    ~server()
    { this->stop(); } 

    void
    start(); // start io & acceptor

    void
    stop(); // stop io & acceptor

    void 
    accept();

  private:
    void
    handle_accept(const boost::system::error_code&, connection::pointer);

    boost::asio::io_service m_ios;
    boost::asio::ip::tcp::acceptor m_acceptor;
  };
  extern std::unique_ptr<server> g_server_ptr;
} // namespace serverd
