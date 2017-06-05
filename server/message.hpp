#pragma once

#include <cstdlib>
#include <ctime>

#include <boost/asio.hpp>

namespace serverd
{
  class version_message
  {
  
  private:
    class net_addr
    {
    private:
      uint32_t m_time;
      // TODO: uint64_t services;
      char ip[16];
      uint16_t m_port;
    };

    int32_t m_version = 0;
    // TODO: uint64_t services;
    int64_t m_timestamp = static_cast<int64_t>(std::time(nullptr));
    int32_t m_height = 12;
    net_addr addr_recv;
    net_addr addr_from;
    uint64_t m_nonce = static_cast<uint64_t>(std::rand());
    bool m_relay = true;
  };
} // namespace ad_patresd
