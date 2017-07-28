#include "./output.h"

Output::Output(size_t blockNum, size_t txeNum)
: blockNumber(blockNum), txeNumber(txeNum)
{
}

bool
Output::operator!=(const Output& v2) const
{
  return blockNumber != v2.blockNumber || txeNumber != v2.txeNumber;
}

bool
Output::operator==(const Output& v2) const
{
  return !(*this != v2);
}

std::ostream&
operator<<(std::ostream& os, const Output& o)
{
  return os << "Output: "
            << "block\t" << o.blockNumber << "\ttx" << o.txeNumber << std::endl;
}