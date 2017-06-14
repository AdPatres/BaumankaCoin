#include "net_addr.hpp"

using namespace messages;

payload_t& 
operator<<(payload_t& payload, const net_addr& obj)
{
  for (auto byte : obj.ip)
    payload.push_back(byte);
  for (auto byte : itobs(obj.port))
    payload.push_back(byte);
  return payload;
}

std::istream& 
operator>>(std::istream& is, messages::net_addr& obj)
{
  is.read(reinterpret_cast<char*>(&obj.ip), sizeof(obj.ip));
  is.read(reinterpret_cast<char*>(&obj.port), sizeof(obj.port));
  return is;
}

bool
operator!=(messages::net_addr lhs, const messages::net_addr rhs)
{ 
  auto lip = boost::asio::ip::address_v4(lhs.ip);
  auto rip = boost::asio::ip::address_v4(rhs.ip);
  return std::tie(lip, lhs.port) != std::tie(rip, rhs.port); 
}

bool
operator==(messages::net_addr lhs, const messages::net_addr rhs)
{ 
  auto lip = boost::asio::ip::address_v4(lhs.ip);
  auto rip = boost::asio::ip::address_v4(rhs.ip);
  return std::tie(lip, lhs.port) == std::tie(rip, rhs.port); 
}

payload_t& 
operator<<(payload_t& payload, const getaddr& obj)
{ return payload; }

std::istream& 
operator>>(std::istream& is, getaddr& obj)
{ return is; }

payload_t& 
operator<<(payload_t& payload, const addr& obj)
{
  for (auto byte : itobl(obj.addr_list.size()))
    payload.push_back(byte);
  for (auto el : obj.addr_list)
    payload << el; 
  return payload; 
}

std::istream& 
operator>>(std::istream& is, addr& obj)
{ 
  uint32_t size = 0;
  is.read(reinterpret_cast<char*>(&size), sizeof(size));
  obj.addr_list = std::vector<messages::net_addr>(size);
  for (size_t i = 0; i < size; ++i)
    {
      messages::net_addr na;
      is >> na;
      obj.addr_list[i] = na;
    }
  return is; 
}
