#include "getblocks.hpp"
#include <botan/hex.h>
#include <cassert>
#include <exception>

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
  char ha[32];
  obj.hash = hash_t(32);
  assert(obj.hash.size() == 32);
  is.read(reinterpret_cast<char*>(ha), 32);
  for (size_t i = 0; i < 32; ++i)
    obj.hash[i] = ha[i];
  return is;
}
