#include "block.hpp"

using namespace messages;

payload_t& 
operator<<(payload_t& payload, const block_message& obj)
{
  for (const auto& byte : itobl(obj.data.size()))
    payload.push_back(byte);
  for (const auto& byte : obj.data)
    payload.push_back(byte);
  return payload; 
}

std::istream& 
operator>>(std::istream& is, block_message& obj)
{
  uint32_t size;
  is.read(reinterpret_cast<char*>(&size), sizeof(size));
  obj.data = payload_t(size);
  is.read(reinterpret_cast<char*>(obj.data.data()), size);
  // for (size_t i = 0; i < obj.data.size(); ++i)
  //   {
  //     uint8_t byte;
  //     is >> byte;
  //     obj.data[i] = byte;
  //   }
  return is;
}
