#pragma once

#ifndef ADDTAIL_H
#define ADDTAIL_H

#include "./command.h"
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

#endif // ADDTAIL_H
