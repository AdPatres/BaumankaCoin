#pragma once
#include "command.h"
class RemoveInput :
	public Command
{
public:
	RemoveInput(Output out, size_t tail, std::vector<uint8_t> hashInfo) :output(out), tailNum(tail), hash(hashInfo)
	{}
	void Execute()
	{
		txe.removeInput(output, tailNum, hash);
	}

	void unExecute()
	{
		txe.addInput(output, tailNum, hash);
	}
	~RemoveInput();
private:
	Output output;
	size_t tailNum;
	std::vector<uint8_t> hash;
};