#include "./addInput.h"

using namespace ad_patres;

AddInput::AddInput(Output out, size_t tail, std::vector<uint8_t> hashInfo)
: output(out), tailNum(tail), hash(hashInfo)
{
}

void
AddInput::Execute()
{
  txe.addInput(output, tailNum, hash);
}

void
AddInput::unExecute()
{
  txe.removeInput(output, tailNum, hash);
}
