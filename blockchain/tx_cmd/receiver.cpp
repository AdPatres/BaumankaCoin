#include "./receiver.h"

using namespace ad_patres;
using namespace Botan;

void
Receiver::Initialize(std::vector<uint8_t> key)
{
  keyPublic = key;
  command->setPublicKey(keyPublic);
}

Transaction
Receiver::get() const
{
  return Command::txe;
}

void
Receiver::addInput(Output output, size_t tailNum, std::vector<uint8_t> info)
{
  command = new AddInput(output, tailNum, info);
  command->Execute();
  DoneCommands.push_back(command);
}

void
Receiver::addTail(Tail tail)
{
  command = new AddTail(tail);
  command->Execute();
  DoneCommands.push_back(command);
}

void
Receiver::removeInput(Output output, size_t tailNum, std::vector<uint8_t> info)
{
  command = new RemoveInput(output, tailNum, info);
  command->Execute();
  DoneCommands.push_back(command);
}

void
Receiver::removeTail(Tail tail)
{
  command = new RemoveTail(tail);
  command->Execute();
  DoneCommands.push_back(command);
}

void
Receiver::sign(ECDSA_PrivateKey privKey)
{
  command = new Signer(privKey);
  command->Execute();
  DoneCommands.push_back(command);
}

void
Receiver::clear()
{
  command = new Cleaner();
  command->Execute();
  DoneCommands.push_back(command);
}

void
Receiver::Undo()
{
  if (DoneCommands.size() == 0)
    std::cout << "There is nothing to undo!" << std::endl;
  else
    {
      command = DoneCommands.back();
      DoneCommands.pop_back();
      command->unExecute();
      delete command;
    }
}
