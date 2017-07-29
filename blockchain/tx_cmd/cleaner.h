#pragma once

#ifndef CLEANER_H
#define CLEANER_H

#include "./command.h"

namespace ad_patres
{
  class Cleaner : public Command
  {
  public:
    Cleaner() = default;

    void
    Execute();

    void
    unExecute();

  private:
    Transaction removed;
  };
}; // namespace ad_patre

#endif // CLEANER_H
