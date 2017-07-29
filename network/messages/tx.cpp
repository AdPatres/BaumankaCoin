#include "./tx.hpp"

using namespace ad_patres::messages;

payload_t&
ad_patres::operator<<(payload_t& payload, const tx& obj)
{
  for (const auto& byte : itobl(obj.data.size()))
    payload.push_back(byte);
  for (const auto& byte : obj.data)
    payload.push_back(byte);
  return payload;
}

std::istream&
ad_patres::operator>>(std::istream& is, tx& obj)
{
  uint32_t size;
  is.read(reinterpret_cast<char*>(&size), sizeof(size));
  obj.data = payload_t(size);
  is.read(reinterpret_cast<char*>(obj.data.data()), size);
  return is;
}
