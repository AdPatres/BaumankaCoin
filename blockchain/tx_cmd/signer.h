#pragma once

#ifndef SIGNER_H
#define SIGNER_H

#include "./command.h"

#include <botan/pubkey.h>

namespace ad_patres
{
  class Signer : public Command
  {
  public:
    Signer(ECDSA_PrivateKey privKey);

    void
    Execute();

    void
    unExecute();

  private:
    ECDSA_PrivateKey key;
  };
}; // namespace ad_patres

#endif // SIGNER_H
