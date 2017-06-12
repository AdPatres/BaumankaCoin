#include "message.hpp"

#include <arpa/inet.h>

using namespace messages;

std::array<uint8_t, sizeof(uint16_t)>
itobs(uint16_t val)
{
  val = static_cast<uint16_t>(::htons(val));
  std::array<uint8_t, sizeof(uint16_t)> arr;
  for (size_t i = arr.size(); i > 0; --i, val >>= 8)
    arr[i-1] = val % 256;
  return std::move(arr);
}

std::array<uint8_t, sizeof(uint32_t)>
itobl(uint32_t val)
{
  val = static_cast<uint32_t>(::htonl(val));
  std::array<uint8_t, sizeof(uint32_t)> arr;
  for (size_t i = arr.size(); i > 0; --i, val >>= 8)
    arr[i-1] = val % 256;
  return std::move(arr);
}
