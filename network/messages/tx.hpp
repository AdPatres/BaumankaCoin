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

  messages::payload_t&
  operator<<(messages::payload_t&, const messages::tx&);

  std::istream&
  operator>>(std::istream&, messages::tx&);
} // namespace ad_patres

#endif // TXMSG_H
