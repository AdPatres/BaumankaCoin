#pragma once

#ifndef GETBLOCKS_H
#define GETBLOCKS_H

#include "./message.hpp"

#include <vector>

namespace ad_patres
{
  namespace messages
  {
    struct getblocks
    {
      hash_t hash;
      const char command[command_size] = "getblocks";
    };
  } // namespace messages
} // namespace ad_patres


ad_patres::messages::payload_t&
operator<<(ad_patres::messages::payload_t&, const ad_patres::messages::getblocks&);

std::istream&
operator>>(std::istream&, ad_patres::messages::getblocks&);

#endif // GETBLOCKS_H
