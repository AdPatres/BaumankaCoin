#pragma once

#ifndef ADDTAIL_H
#define ADDTAIL_H

#include "./command.h"

namespace ad_patres
{
  class AddTail : public Command
  {
  public:
    AddTail(Tail tailObj) : tail(tailObj) {}
    void
    Execute()
    {
      txe.addTail(tail);
    }

    void
    unExecute()
    {
      txe.removeTail(tail);
    }

  private:
    Tail tail;
  };
}; // namespace ad_patres

#endif // ADDTAIL_H
