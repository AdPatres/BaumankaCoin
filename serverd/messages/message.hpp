#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

#include <boost/asio/streambuf.hpp>
#include <botan/botan.h>

namespace messages
{
  using payload_t = std::vector<uint8_t>; // bytes
  using hash_t = Botan::secure_vector<uint8_t>;

  const uint8_t command_size = 12;

  template<typename Message>
    Message
    create(const payload_t& payload)
    {
      boost::asio::streambuf buf;
      std::iostream ios(&buf);
      for (auto byte : payload)
        ios << byte;
      Message msg;
      ios >> msg;
      return std::move(msg);
    }

  template<class Container>
    payload_t
    to_array(const Container& arr)
    {
      payload_t payload;
      for (const auto& el : arr)
        payload << el;
      return std::move(payload);
    }
} // namespace messages

std::array<uint8_t, sizeof(uint16_t)>
itobs(uint16_t val);

std::array<uint8_t, sizeof(uint32_t)>
itobl(uint32_t val);
