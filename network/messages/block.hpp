#pragma once

#ifndef BLOCKMSG_H
#define BLOCKMSG_H

#include "./message.hpp"

namespace ad_patres
{
  namespace messages
  {
    struct block_message
    {
      payload_t data;
      const char command[command_size] = "block";
    };
  } // namespace messages
} // namespace ad_patres

ad_patres::messages::payload_t&
operator<<(ad_patres::messages::payload_t&, const ad_patres::messages::block_message&);

std::istream&
operator>>(std::istream&, ad_patres::messages::block_message&);

#endif // BLOCKMSG_H
