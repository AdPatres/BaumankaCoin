#pragma once

#include <cstdlib>
#include <ctime>
#include <string>

#include <boost/asio.hpp>
#include <boost/serialization/array.hpp>

namespace serverd
{
  class net_addr
  {
  private:
    friend class boost::serialization::access;

  public:
    explicit
    net_addr(boost::asio::ip::tcp::endpoint);

  private:
    template<class Archive>
      void serialize(Archive & ar, const unsigned int version)
      {
          ar & m_ip;
          ar & m_port;
      }

    boost::asio::ip::address_v4::bytes_type m_ip;
    uint16_t                                m_port;
  };

  class version_message
  {
  private:
    friend class boost::serialization::access;

    template<class Archive>
      void serialize(Archive & ar, const unsigned int version)
      {
          ar & m_height;
          ar & m_addr_recv;
          ar & m_addr_from;
          ar & m_nonce;
      }

    int32_t   m_height = 12;
    net_addr  m_addr_recv;
    net_addr  m_addr_from;
    uint64_t  m_nonce = static_cast<uint64_t>(std::rand());
  };
} // namespace ad_patresd
