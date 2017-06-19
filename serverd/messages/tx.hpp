#pragma once

#include "message.hpp"

namespace messages
{
  struct tx
  {
    payload_t   data;
    const char  command[command_size] = "tx";
  };
} // namespace messages

messages::payload_t& 
operator<<(messages::payload_t&, const messages::tx&);

std::istream& 
operator>>(std::istream&, messages::tx&);
