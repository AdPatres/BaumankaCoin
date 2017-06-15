#pragma once

#include "message.hpp"

#include <cstdlib>
#include <vector>

namespace messages
{
  struct inv_vect
  {
    enum class inv_type
    {
      error     = 0,
      msg_tx    = 1,
      msg_block = 2,
    } type;
    hash_t    hash;
  };

  struct inv
  {
    std::vector<inv_vect> inventory;
    const char            command[command_size] = "inv";
  };

  struct getdata
  {
    std::vector<inv_vect> inventory;
    const char            command[command_size] = "getdata";
  };
} // namespace messages

messages::payload_t& 
operator<<(messages::payload_t&, const messages::inv_vect&);

std::istream& 
operator>>(std::istream&, messages::inv_vect&);

messages::payload_t& 
operator<<(messages::payload_t&, const messages::inv&);

std::istream& 
operator>>(std::istream&, messages::inv&);

messages::payload_t& 
operator<<(messages::payload_t&, const messages::getdata&);

std::istream& 
operator>>(std::istream&, messages::getdata&);
