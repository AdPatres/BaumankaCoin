#pragma once

#include "message.hpp"

#include <vector>

namespace messages
{
  using blockhash_t = std::vector<uint8_t>; 

  struct getblocks
  {
    blockhash_t hash;
    const char  command[command_size] = "getblocks";
  };
} // namespace messages


messages::payload_t& 
operator<<(messages::payload_t&, const messages::getblocks&);

std::istream& 
operator>>(std::istream&, messages::getblocks&);
