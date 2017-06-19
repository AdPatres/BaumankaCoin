#pragma once
#include "command.h"
#include <botan/pubkey.h>
class Signer :
	public Command
{
public:
	Signer(ECDSA_PrivateKey privKey) :key(privKey)
{}
	   void Execute()
	   {
		   txe.sign(key);
	   }

	   void unExecute()
	   {
		   txe.removeSign();
	   }
	   ~Signer();
private:
	ECDSA_PrivateKey key;
};