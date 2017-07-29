#pragma once

#ifndef REMOVETAIL_H
#define REMOVETAIL_H

#include "./command.h"

namespace ad_patres
{
  class RemoveTail : public Command
  {
  public:
    RemoveTail(Tail);

    void
    Execute();

    void
    unExecute();

  private:
    Tail tail;
  };
}; // namespace ad_patres

#endif // REMOVETAIL_H
