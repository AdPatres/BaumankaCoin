#pragma once

#ifndef TXMSG_H
#define TXMSG_H

#include "./message.hpp"

namespace ad_patres
{
  namespace messages
  {
    struct tx
    {
      payload_t data;
      const char command[command_size] = "tx";
    };
  } // namespace messages
} // namespace ad_patres

ad_patres::messages::payload_t&
operator<<(ad_patres::messages::payload_t&, const ad_patres::messages::tx&);

std::istream&
operator>>(std::istream&, ad_patres::messages::tx&);

#endif // TXMSG_H
