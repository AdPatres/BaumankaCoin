#pragma once
#include "command.h"
class Cleaner :
	public Command
{
public:
	Cleaner()=default;
	void Execute()
	{
		std::swap(txe, removed);
	}

	void unExecute()
	{
		std::swap(txe, removed);
	}
	~Cleaner();
private:
	Transaction removed;
};
