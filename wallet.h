#pragma once

#ifndef WALLET_H
#define WALLET_H

#include "./blockchain/blockchain.h"
#include "./blockchain/tx_cmd/receiver.h"
#include "./miner.hpp"
#include "./network/server.hpp"

#include <botan/alg_id.h>   // to intialize
#include <botan/auto_rng.h> // rand numb gen
#include <botan/ec_group.h> // groups of Eliptic curves
#include <botan/ecdsa.h>    // privateKey on Eliptic Curve
#include <botan/hash.h>     // with sha2 for hashing
#include <botan/hex.h>      // to std::out from hex
#include <botan/pkcs8.h>    // privkey in strings and vector
#include <botan/pubkey.h>   // PK_SIGNER
#include <botan/secmem.h>   // secure_vector
#include <botan/sha2_32.h>
#include <botan/x509_key.h> // pubkey in strings and vector

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

namespace ad_patres
{
  class Wallet
  {
  public:
    Wallet();

    Wallet(Botan::secure_vector<uint8_t>, std::vector<uint8_t>);

    ~Wallet();

    Wallet&
    operator=(const Wallet&);

    Botan::secure_vector<uint8_t>
    getAddress() const;

    std::vector<Botan::secure_vector<uint8_t>> getHashesAfter(uint64_t) const;

    void
    setAvailibleForAdress();

    void
    setCurrentSum();

    void
    createTxe(std::istream&, std::ostream&);

    void
    changeMiner();

    void
    welcome();

    void
    commandProg();

  private:
    void
    readInputs(std::istream&, std::ostream&);

    void
    readTails(std::istream&, std::ostream&);

    Botan::AutoSeeded_RNG aga;
    Botan::EC_Group faf = Botan::EC_Group("secp256k1");
    Botan::secure_vector<uint8_t> encPrivateKey;
    std::vector<uint8_t> publicKey;
    Botan::ECDSA_PrivateKey thePrivateKey = Botan::ECDSA_PrivateKey(aga, faf);

    Botan::secure_vector<uint8_t> address;
    size_t sum = 0;
    Receiver receiver;

    std::shared_ptr<Blockchain> chain;
    std::vector<AddedOutput> availibleForAddress;

    server m_server;
    miner m_miner;
  };
}; // namespace ad_patres

#endif // WALLET_H
