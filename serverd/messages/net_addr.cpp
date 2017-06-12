#include "net_addr.hpp"

using namespace messages;

payload_t& 
operator<<(payload_t& vec, const net_addr& obj)
{
  for (auto byte : obj.ip)
    vec.push_back(byte);
  for (auto byte : itobs(obj.port))
    vec.push_back(byte);
  return vec;
}

std::istream& 
operator>>(std::istream& is, messages::net_addr& obj)
{
  is.read(reinterpret_cast<char*>(&obj.ip), sizeof(obj.ip));
  is.read(reinterpret_cast<char*>(&obj.port), sizeof(obj.port));
  return is;
}
