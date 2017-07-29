#pragma once

#ifndef COMMAND_H
#define COMMAND_H

#include "../transaction.h"

#include <botan/botan.h>

namespace ad_patres
{
  class Command
  {
  public:
    virtual void
    Execute() = 0;

    virtual void
    unExecute() = 0;

    void
    setPublicKey(std::vector<uint8_t> key);

    virtual ~Command() = default;

    static Transaction txe;
  };
}; // namespace ad_patres

#endif // COMMAND_H
