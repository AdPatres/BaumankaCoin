#pragma once

#ifndef VERSION_H
#define VERSION_H

#include "./message.hpp"
#include "./net_addr.hpp"

#include <cstdint>

#include <boost/asio/ip/tcp.hpp>

namespace ad_patres
{
  namespace messages
  {
    class version // version message
    {
    public:
      version() = default;
      version(boost::asio::ip::tcp::endpoint, uint16_t);

      const char command[command_size] = "version";
      net_addr addr_recv;
      net_addr addr_from;
      uint32_t nonce;
    };

    struct verack
    {
      const char command[command_size] = "verack";
    };
  } // namespace messages
} // namespace ad_patres

ad_patres::messages::payload_t&
operator<<(ad_patres::messages::payload_t&, const ad_patres::messages::version&);

std::istream&
operator>>(std::istream&, ad_patres::messages::version&);

ad_patres::messages::payload_t&
operator<<(ad_patres::messages::payload_t&, const ad_patres::messages::verack&);

std::istream&
operator>>(std::istream&, ad_patres::messages::verack&);

#endif // VERSION_H
