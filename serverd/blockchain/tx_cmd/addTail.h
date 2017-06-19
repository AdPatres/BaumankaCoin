#pragma once
#include "command.h"
class AddTail :
	public Command
{
public:
	AddTail(Tail tailObj) :tail(tailObj)
	{}
	void Execute()
	{
		txe.addTail(tail);
	}

	void unExecute()
	{
		txe.removeTail(tail);
	}
	~AddTail();
private:
	Tail tail;
};

