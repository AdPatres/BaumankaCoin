#pragma once

#include "peer.hpp"

#include <boost/asio.hpp>

namespace ad_patresd
{
  class server
  {
  public:
    server(unsigned short);

    ~server();

    void
    start_accept();

    void
    handle_accept(const boost::system::error_code&, peer::pointer);

    void
    stop();

  private:
    boost::asio::io_service m_ios;
    boost::asio::ip::tcp::acceptor m_acceptor;
  };
} // namespace ad_patresd
