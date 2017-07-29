#include "./command.h"

using namespace ad_patres;

Transaction Command::txe;

void
Command::setPublicKey(std::vector<uint8_t> key)
{
  txe = Transaction(key);
}
