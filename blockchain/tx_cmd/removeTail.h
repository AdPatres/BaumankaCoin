#pragma once
#include "command.h"
class RemoveTail :
	public Command
{
public:
	RemoveTail(Tail tailObj) :tail(tailObj)
	{}
	void Execute()
	{
		txe.removeTail(tail);
	}

	void unExecute()
	{
		txe.addTail(tail);
	}
	~RemoveTail();
private:
	Tail tail;
};

