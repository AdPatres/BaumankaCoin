#pragma once

#include "message.hpp"

#include <boost/asio/ip/address_v4.hpp>

namespace messages
{
  struct net_addr
  {
    boost::asio::ip::address_v4::bytes_type ip;
    uint16_t                                port = 8333;
  };
}

messages::payload_t& 
operator<<(messages::payload_t&, const messages::net_addr&);

std::istream& 
operator>>(std::istream&, messages::net_addr&);
