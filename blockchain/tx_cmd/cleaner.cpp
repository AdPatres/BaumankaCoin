#include "./cleaner.h"

using namespace ad_patres;

void
Cleaner::Execute()
{
  std::swap(txe, removed);
}

void
Cleaner::unExecute()
{
  std::swap(txe, removed);
}
