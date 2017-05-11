#pragma once


#ifndef NETWORK_H
#define NETWORK_H

#define DEFAULT_PORT 8883


#include <boost/asio.hpp>
#include <memory>
#include <string>

using std::string;



namespace network 
{
  class peer
  {
  public:
    using pointer = std::shared_ptr<peer>;

    static inline pointer
    create(boost::asio::io_service& ios) 
    { return pointer(new peer(ios)); }

    inline boost::asio::ip::tcp::socket&
    socket() { return _M_socket; }

    void
    write(const string&);

    void
    write_handler(const boost::system::error_code&, std::size_t);

  private:
    peer(boost::asio::io_service&);

    boost::asio::ip::tcp::socket _M_socket;
    
  };


  class server 
  {
  public:
    server(boost::asio::io_service&);

    void
    start_accept();

    void
    handle_accept(const boost::system::error_code&, peer::pointer);

  private:
    boost::asio::ip::tcp::acceptor _M_acceptor;
  };
} // namespace network



#endif // NETWORK_H
