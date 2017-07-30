#pragma once

#ifndef TRANSACTON_H
#define TRANSACTON_H

#include "./Input.h"
#include "./tail.h"

#include <memory>
#include <shared_mutex>
#include <vector>

#include <botan/ecdsa.h>

namespace ad_patres
{
  extern std::shared_ptr<std::shared_timed_mutex> BlockchainMutex;
  extern std::shared_ptr<std::shared_timed_mutex> TransactionsMutex;

  class Blockchain;
  class Wallet;

  struct AddedOutput
  {
    AddedOutput() = default;
    AddedOutput(Output out);
    AddedOutput(Output out, size_t tailsSize);

    Output output;
    std::vector<bool> usedTails;
  };

  class Transaction
  {
    friend Blockchain;
    friend Wallet;

  public:
    Transaction() = default;

    Transaction(std::vector<uint8_t> pubKey);

    ~Transaction();

    bool
    operator==(const Transaction&);

    std::vector<uint8_t>
    getBroadcastData() const;

    std::vector<uint8_t>
    getTxeData() const;

    void
    clear();

    bool addInput(Output, size_t, std::vector<uint8_t>);

    bool removeInput(Output, size_t, std::vector<uint8_t>);

    bool addTail(Tail);

    bool removeTail(Tail);

    void sign(Botan::ECDSA_PrivateKey);

    bool addAvailibleTxe(Output, size_t);

    bool
    scanBroadcastedData(std::vector<uint8_t> data, uint32_t& position);

    void
    showInfo() const;
    bool
    removeSign();

  protected:
    std::vector<Input> inputs;
    std::vector<Tail> tails;
    std::vector<uint8_t> pubKey = std::vector<uint8_t>(279, 0);
    std::vector<uint8_t> signature = std::vector<uint8_t>(64, 0);
    static std::vector<AddedOutput> availibleTxes;
  };
}; // namespace ad_patres

#endif // TRANSACTON_H
