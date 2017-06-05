#pragma once

#include <memory>

#include <boost/asio.hpp>

namespace serverd
{
  class peer
  {
  public:
    using pointer = std::shared_ptr<peer>;

    static inline pointer
    create(boost::asio::io_service& ios) 
    { return pointer(new peer(ios)); }

    inline boost::asio::ip::tcp::socket&
    socket() { return m_socket; }

    void
    write(const std::string&);

    void
    send_buffer(const boost::asio::streambuf& msg);

    void
    write_handler(const boost::system::error_code&, std::size_t);

  private:
    peer(boost::asio::io_service&);

    boost::asio::ip::tcp::socket m_socket;
  };
} // namespace ad_patresd
