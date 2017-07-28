#pragma once

#ifndef CLEANER_H
#define CLEANER_H

#include "./command.h"

class Cleaner : public Command
{
public:
  Cleaner() = default;
  void
  Execute()
  {
    std::swap(txe, removed);
  }

  void
  unExecute()
  {
    std::swap(txe, removed);
  }

private:
  Transaction removed;
};

#endif // CLEANER_H
