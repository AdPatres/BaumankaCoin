#pragma once

#ifndef RECEIVER_H
#define RECEIVER_H

#include "./addInput.h"
#include "./addTail.h"
#include "./cleaner.h"
#include "./removeInput.h"
#include "./removeTail.h"
#include "./signer.h"

#include <vector>

#include <botan/ecdsa.h>

namespace ad_patres
{
class Receiver
{
  std::vector<Command*> DoneCommands;
  std::vector<uint8_t> keyPublic;
  Command* command;

public:
  void
  Initialize(std::vector<uint8_t> key);

  Transaction
  get() const;

  void
  addInput(Output output, size_t tailNum, std::vector<uint8_t> info);

  void
  addTail(Tail tail);

  void
  removeInput(Output output, size_t tailNum, std::vector<uint8_t> info);

  void
  removeTail(Tail tail);
  
  void
  sign(Botan::ECDSA_PrivateKey privKey);

  void
  clear();

  void
  Undo();
};
};

#endif // RECEIVER_H
