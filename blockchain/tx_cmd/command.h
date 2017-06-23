#pragma once
#include "../transaction.h"
#include <botan/botan.h>
class Command
{
	
public:
	virtual void Execute() = 0;
	virtual void unExecute() = 0;

	void setPublicKey(std::vector<byte> key)
	{
		txe = Transaction(key);
	}

	virtual ~Command(){}
	static Transaction txe;
};