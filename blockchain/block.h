#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include "./transaction.h"

#include <cstdlib> //itoa
#include <cstring> //memset
#include <iostream>
#include <vector>

#include <botan/botan.h>
#include <botan/hex.h>
#include <botan/sha2_32.h>

namespace ad_patres 
{
  class Block
  {
    friend Blockchain;
    friend Wallet;

  public:
    Block() = default;
    
    ~Block();

    std::vector<uint8_t>
    getBlockData() const;

    std::vector<uint8_t> getTxeData(size_t) const;

    std::vector<uint8_t>
    getBroadcastData() const;

    uint32_t
    getCurrentNumber() const;

    bool
    addFirstTxe(secure_vector<byte> address);

    bool addTransaction(size_t = nonValidated.size());

    bool setPrevBlock(std::vector<uint8_t>);

    bool
    setMerkleRoot();

    void
    calculate();

    void
    broadcastBlock();

    bool
    scanBroadcastedData(std::vector<uint8_t>, uint32_t&);

    void
    setNumber(size_t num);

    void
    showInfo() const;

    static std::vector<Transaction> nonValidated;

  protected:
    void
    setHash(std::vector<byte> from, uint32_t& position,
            secure_vector<uint8_t>& to);
            
    size_t version = 1;                                         // default value
    secure_vector<byte> prevBlock = secure_vector<byte>(32, 0); // hash
    size_t currentNumber = 0;
    secure_vector<byte> merkleRoot = secure_vector<byte>(32, 0);
    size_t bits = 1;  // Proof of work difficulty
    size_t nonce = 0; // to change hash
    size_t txsCount = 0; // hash untill this
    std::vector<Transaction> txs; // static pool for non validated txes
  };
}; // namespace ad_patres

#endif // BLOCK_H
