#pragma once

#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "./block.h"

#include <memory>
#include <vector>

#include <botan/secmem.h>

namespace ad_patres
{
  class Blockchain
  {
    friend Wallet;

  public:
    ~Blockchain() = default;

    void
    getBlockchain();

    void
    getNonValidatedTxes();

    bool
    validateBlockChain();

    size_t
    size() const;

    bool
    addBlock(Block&);

    void
    customize(size_t numberOfBlocks, Botan::secure_vector<uint8_t> address);

    static std::shared_ptr<Blockchain>
    instance();

    bool
    validateTxn(const Transaction&, std::vector<std::pair<Output, size_t>>);

    Botan::secure_vector<uint8_t>
    getLastBlockHash();

    uint32_t
    getBlockchainSize() const;

    std::vector<Block> getBlocksAfter(uint64_t) const;

    int64_t findByHash(Botan::secure_vector<uint8_t>);

    void
    addTx(const Transaction& tx);

    std::vector<uint8_t>
    getLastBlockData() const;

  private:
    Blockchain() = default;

    static std::shared_ptr<Blockchain> _self;

    size_t bits = 1;

    bool
    validateBlock(Block&);

    bool
    validateMerkleRoot(const Block&);

    bool
    validateFirstTxn(const Transaction&);

    // returns amount of money taken from inputs
    size_t
    validateInputs(const Transaction&, std::vector<std::pair<Output, size_t>>);

    bool
    validateSignature(const Transaction&);

    bool
    validateTails(const Transaction&, size_t);

    void restore(std::vector<std::pair<Output, size_t>>);

    void
    setAvailibleTxes(Block&);

    void
    clearAvailibleTxes();

    void clearNonValidated(Block);

    std::vector<Block> blockChain;
    std::vector<Block> nonValidatedBlockChain;
  };
};

#endif // BLOCKCHAIN_H
