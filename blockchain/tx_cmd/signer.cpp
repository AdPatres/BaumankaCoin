#include "./signer.h"

using namespace ad_patres;

Signer::Signer(ECDSA_PrivateKey privKey) : key(privKey) {}

void
Signer::Execute()
{
  txe.sign(key);
}

void
Signer::unExecute()
{
  txe.removeSign();
}
