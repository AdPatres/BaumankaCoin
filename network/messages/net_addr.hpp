#pragma once

#ifndef NETADDR_H
#define NETADDR_H 

#include "./message.hpp"

#include <vector>

#include <boost/asio/ip/address_v4.hpp>

namespace ad_patres
{
  namespace messages
  {
    struct net_addr
    {
      boost::asio::ip::address_v4::bytes_type ip;
      uint16_t port;
    };

    struct getaddr
    {
      const char command[command_size] = "getaddr";
    };

    struct addr
    {
      const char command[command_size] = "addr";
      std::vector<net_addr> addr_list;
    };
  }
}

ad_patres::messages::payload_t&
operator<<(ad_patres::messages::payload_t&, const ad_patres::messages::net_addr&);

std::istream&
operator>>(std::istream&, ad_patres::messages::net_addr&);

bool
operator!=(ad_patres::messages::net_addr lhs, const ad_patres::messages::net_addr rhs);

bool
operator==(ad_patres::messages::net_addr lhs, const ad_patres::messages::net_addr rhs);

ad_patres::messages::payload_t&
operator<<(ad_patres::messages::payload_t&, const ad_patres::messages::getaddr&);

std::istream&
operator>>(std::istream&, ad_patres::messages::getaddr&);

ad_patres::messages::payload_t&
operator<<(ad_patres::messages::payload_t&, const ad_patres::messages::addr&);

std::istream&
operator>>(std::istream&, ad_patres::messages::addr&);

#endif // NETADDR_H
