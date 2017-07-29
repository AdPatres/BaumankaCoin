#include "./command.h"

using namespace ad_patres;

Transaction Command::txe;

void
Command::setPublicKey(std::vector<byte> key)
{
  txe = Transaction(key);
}
