#pragma once

#ifndef SERVER_H
#define SERVER_H

#include "connection.hpp"
#include "./messages/block.hpp"
#include "./messages/inv.hpp"
#include "./messages/net_addr.hpp"
#include "./messages/tx.hpp"
#include "../blockchain/blockchain.h"

#include <cstdint>      // uint16_t
#include <forward_list>
#include <functional>   // function
#include <memory>       // unique_ptr
#include <thread>
#include <string>       // string

#include <boost/asio/ip/tcp.hpp> // acceptor
#include <boost/bind.hpp> 

namespace serverd
{
  class server
  {
  public:
    using ConnectHandler = 
      std::function<void(const boost::system::error_code&, connection::pointer)>;

    server();

    ~server();

    void
    start();

    void
    stop();

    void
    share(const Block&);

    void
    share(const Transaction&);

  private:
    void
    m_accept();

    void
    m_run();

    void 
    m_load(const std::string& filename = "adpatres.conf");

    void
    m_connect(const boost::asio::ip::tcp::endpoint&, ConnectHandler);

    void
    m_handle_accept(const boost::system::error_code&, connection::pointer);

    messages::addr
    m_make_addr();

    void
    m_handshake    (const boost::system::error_code&, connection::pointer);

    void
    m_handle_inv    (connection::pointer, const messages::inv&);

    void
    m_handle_version(connection::pointer, auto);
    
    template<class T>
      void
      m_share_handler(T&, connection::pointer
      );

    boost::asio::io_service         m_ios;
    boost::asio::ip::tcp::acceptor  m_acceptor;
    uint16_t                        m_port;

    std::forward_list<messages::net_addr> m_peers;
    std::shared_ptr<Blockchain>           m_blockchain_ptr =  Blockchain::instance();

    std::unique_ptr<std::thread>  m_acc_th_ptr;
  };

  // template<>
  //   void
  //   server::m_share_handler<Transaction>(const boost::system::error_code&, 
  //     connection::pointer, Transaction&);

  // template<>
  //   void
  //   server::m_share_handler<Block>(Block&, 
  //     connection::pointer);
} // namespace serverd

#endif // SERVER_H
