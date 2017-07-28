#pragma once

#ifndef SIGNER_H
#define SIGNER_H

#include "./command.h"

#include <botan/pubkey.h>

class Signer : public Command
{
public:
  Signer(ECDSA_PrivateKey privKey) : key(privKey) {}
  void
  Execute()
  {
    txe.sign(key);
  }

  void
  unExecute()
  {
    txe.removeSign();
  }

private:
  ECDSA_PrivateKey key;
};

#endif // SIGNER_H
