#include "getblocks.hpp"

using namespace messages;

payload_t& 
operator<<(payload_t& payload, const getblocks& obj)
{
  for (const auto& byte : obj.hash)
    payload.push_back(byte);
  return payload; 
}

std::istream&
operator>>(std::istream& is, getblocks& obj)
{
  obj.hash = blockhash_t(32);
  for (size_t i = 0; i < obj.hash.size(); ++i)
    {
      uint8_t byte;
      is >> byte;
      obj.hash[i] = byte;
    }
  return is;
}
