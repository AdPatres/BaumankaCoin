#include "./removeInput.h"

using namespace ad_patres;

RemoveInput::RemoveInput(Output out, size_t tail, std::vector<uint8_t> hashInfo)
: output(out), tailNum(tail), hash(hashInfo)
{
}

void
RemoveInput::Execute()
{
  txe.removeInput(output, tailNum, hash);
}

void
RemoveInput::unExecute()
{
  txe.addInput(output, tailNum, hash);
}
