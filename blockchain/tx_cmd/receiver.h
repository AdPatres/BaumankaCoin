#pragma once
#include "addInput.h"
#include "addTail.h"
#include "removeInput.h"
#include "removeTail.h"
#include "signer.h"
#include "cleaner.h"
class Receiver
{
	std::vector<Command*> DoneCommands;
	std::vector<byte> keyPublic;
	Command* command;
public:
	void Initialize(std::vector<byte> key)
	{
		keyPublic = key;
		command->setPublicKey(keyPublic);
	}
	void addInput(Output output, size_t tailNum, std::vector<uint8_t> info)
	{
		command = new AddInput(output, tailNum, info);
		command->Execute();
		DoneCommands.push_back(command);
	}
	void addTail(Tail tail)
	{
		command = new AddTail(tail);
		command->Execute();
		DoneCommands.push_back(command);
	}
	void removeInput(Output output, size_t tailNum, std::vector<uint8_t> info)
	{
		command = new RemoveInput(output, tailNum, info);
		command->Execute();
		DoneCommands.push_back(command);
	}
	void removeTail(Tail tail)
	{
		command = new RemoveTail(tail);
		command->Execute();
		DoneCommands.push_back(command);
	}
	void sign(ECDSA_PrivateKey privKey)
	{
		command = new Signer(privKey);
		command->Execute();
		DoneCommands.push_back(command);
	}
	void clear()
	{
		command = new Cleaner();
		command->Execute();
		DoneCommands.push_back(command);
	}
	void Undo()
	{
		if (DoneCommands.size() == 0)
		{
			std::cout << "There is nothing to undo!" << std::endl;
		}
		else
		{
			command = DoneCommands.back();
			DoneCommands.pop_back();
			command->unExecute();
			delete command;
		}
	}
	Transaction get()
	{
		return Command::txe;
	}
};
