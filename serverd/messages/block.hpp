#include "message.hpp"

namespace messages
{
  struct block_message
  {
    payload_t   data;
    const char  command[command_size] = "block"; 
  };
} // namespace messages

messages::payload_t& 
operator<<(messages::payload_t&, const messages::block_message&);

std::istream& 
operator>>(std::istream&, messages::block_message&);
