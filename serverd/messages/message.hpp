#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/asio.hpp>
#include <boost/serialization/vector.hpp>

namespace messages
{
  class message
  {
  public:
    template<typename Payload>
      message(const std::string&, const Payload& payload);

  private:
    friend class boost::serialization::access;
    template<class Archive>
      void 
      serialize(Archive & ar, const unsigned int version)
      {
        ar & m_command;
        ar & m_payload;
      }

    std::string           m_command;
    std::vector<uint8_t>  m_payload;
  };

   template<typename Payload>
    message::message(const std::string& cmd, const Payload& payload)
    : m_command(cmd)
    {
      boost::asio::streambuf stbuf;
      std::iostream iost(&stbuf);
      iost << payload;
      std::cerr << "echoed\n";
      while (iost)
        {
          uint8_t byte;
          iost >> byte;
          m_payload.push_back(byte);
        }
    }
} // namespace messages
