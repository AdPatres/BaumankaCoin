#pragma once

#ifndef SIGNER_H
#define SIGNER_H

#include "./command.h"

#include <botan/ecdsa.h>
#include <botan/pubkey.h>

namespace ad_patres
{
  class Signer : public Command
  {
  public:
    Signer(Botan::ECDSA_PrivateKey);

    void
    Execute();

    void
    unExecute();

  private:
    Botan::ECDSA_PrivateKey key;
  };
}; // namespace ad_patres

#endif // SIGNER_H
