#pragma once

#ifndef REMOVETAIL_H
#define REMOVETAIL_H

#include "command.h"
class RemoveTail : public Command
{
public:
  RemoveTail(Tail tailObj) : tail(tailObj) {}
  void
  Execute()
  {
    txe.removeTail(tail);
  }

  void
  unExecute()
  {
    txe.addTail(tail);
  }

private:
  Tail tail;
};

#endif // REMOVETAIL_H
