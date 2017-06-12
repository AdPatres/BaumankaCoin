#pragma once

#include "message.hpp"
#include "net_addr.hpp"

#include <cstdint>

#include <boost/asio/ip/tcp.hpp>

namespace messages
{
  class version // version message
  {
  public:
    version() = default;
    version(boost::asio::ip::tcp::endpoint, uint16_t);

    const char  command[command_size] = "version";
    net_addr    addr_recv;
    net_addr    addr_from;
    uint32_t    nonce;
  };

  struct verack
  { const char  command[command_size] = "verack"; };
} // namespace messages

messages::payload_t& 
operator<<(messages::payload_t&, const messages::version&);

std::istream&
operator>>(std::istream&, messages::version&);

messages::payload_t& 
operator<<(messages::payload_t&, const messages::verack&);

std::istream& 
operator>>(std::istream&, messages::verack&);
