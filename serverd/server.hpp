#pragma once

#include "connection.hpp"
#include "messages/net_addr.hpp"
#include "blockchain/wallet.h"

#include <cstdint>      // uint16_t
#include <forward_list>
#include <functional>   // function
#include <memory>       // unique_ptr
#include <vector>

#include <boost/asio.hpp> 
#include <boost/bind.hpp>

namespace serverd
{
  class server
  {
  public:
    using ConnectHandler = 
      std::function<void(const boost::system::error_code&, connection::pointer)>;

    explicit
    server(uint16_t);

    ~server()
    { this->stop(); } 

    void
    start(); // start io & acceptor

    void
    stop(); // stop io & acceptor

    void 
    accept(); // listen messages

    void
    connect(const boost::asio::ip::tcp::endpoint& endp, ConnectHandler cb);

  private:
    void
    m_handle_accept (const boost::system::error_code&, connection::pointer);

    void
    m_handshake     (const boost::system::error_code&, connection::pointer);

    messages::addr
    m_make_addr();

    boost::asio::io_service               m_ios;
    boost::asio::ip::tcp::acceptor        m_acceptor;

    std::forward_list<messages::net_addr> m_peers;

    Wallet                                m_wallet;
  };
  extern std::unique_ptr<server> g_server_ptr;
} // namespace serverd
