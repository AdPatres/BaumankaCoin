#pragma once

#ifndef COMMAND_H
#define COMMAND_H

#include "../transaction.h"
#include <botan/botan.h>
class Command
{
public:
  virtual void
  Execute()
    = 0;
  virtual void
  unExecute()
    = 0;

  void
  setPublicKey(std::vector<byte> key)
  {
    txe = Transaction(key);
  }

  virtual ~Command() {}
  static Transaction txe;
};

#endif // COMMAND_H
