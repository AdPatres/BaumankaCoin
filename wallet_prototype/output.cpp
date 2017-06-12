#include "stdafx.h"
#include "output.h"



Output::Output(size_t blockNum, size_t txeNum):blockNumber(blockNum), txeNumber(txeNum)
{
}
bool Output::operator !=(const Output& v2) const
{
	if (blockNumber != v2.blockNumber || txeNumber != v2.txeNumber)
		return true;
	return false;
}
bool Output::operator ==(const Output& v2) const
{
	return !(*this!=v2);
}