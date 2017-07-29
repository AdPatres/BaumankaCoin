#include "./removeTail.h"

using namespace ad_patres;

RemoveTail::RemoveTail(Tail tailObj) : tail(tailObj) {}

void
RemoveTail::Execute()
{
  txe.removeTail(tail);
}

void
RemoveTail::unExecute()
{
  txe.addTail(tail);
}
