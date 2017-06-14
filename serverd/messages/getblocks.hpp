#pragma once

#include "message.hpp"

#include <botan/botan.h>

#include <vector>

namespace messages
{
  using blockhash_t = Botan::secure_vector<unsigned char>; 

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
