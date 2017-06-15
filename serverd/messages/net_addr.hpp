#pragma once

#include "message.hpp"

#include <vector>

#include <boost/asio/ip/address_v4.hpp>

namespace messages
{
  struct net_addr
  {
    boost::asio::ip::address_v4::bytes_type ip;
    uint16_t                                port;
  };

  struct getaddr
  { const char command[command_size] = "getaddr"; };

  struct addr
  {
    const char            command[command_size] = "addr";
    std::vector<net_addr> addr_list;
  };
}

messages::payload_t& 
operator<<(messages::payload_t&, const messages::net_addr&);

std::istream& 
operator>>(std::istream&, messages::net_addr&);

bool
operator!=(messages::net_addr lhs, const messages::net_addr rhs);

bool
operator==(messages::net_addr lhs, const messages::net_addr rhs);

messages::payload_t& 
operator<<(messages::payload_t&, const messages::getaddr&);

std::istream& 
operator>>(std::istream&, messages::getaddr&);

messages::payload_t& 
operator<<(messages::payload_t&, const messages::addr&);

std::istream& 
operator>>(std::istream&, messages::addr&);
