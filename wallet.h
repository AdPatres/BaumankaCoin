#pragma once
#include <botan/botan.h>//secure_vector
#include <botan/auto_rng.h>//rand numb gen
#include <botan/ecdsa.h>//privateKey on Eliptic Curve
#include <botan/pkcs8.h>//privkey in strings and vector
#include <botan/x509_key.h>//pubkey in strings and vector
#include <botan/sha2_32.h>
#include <botan/pubkey.h>//PK_SIGNER
#include <botan/ec_group.h>//groups of Eliptic curves
#include <botan/hex.h>//to std::out from hex
#include <botan/alg_id.h>//to intialize
#include <botan/hash.h>//with sha2 for hashing
#include "../blockchain/blockchain.h"
#include <iostream>
#include <vector>
#include "../blockchain/tx_cmd/receiver.h"
#include <memory>

#include "./network/server.hpp"
#include "./miner.hpp"

using namespace Botan;

class Wallet
{
public:
	Wallet();
	Wallet(secure_vector<byte>, std::vector<byte>);
	Wallet& operator=(const Wallet& );
	void setAvailibleForAdress();
	void setCurrentSum();
	void createTxe(std::istream& is, std::ostream& os);
	std::vector<secure_vector<byte>> getHashesAfter(uint64_t) const;
	~Wallet();
	secure_vector<byte> getAddress() { return address; }
	void changeMiner();
	void welcome();
	void commandProg();

private:
	void readInputs(std::istream& is, std::ostream& os);
	void readTails(std::istream& is, std::ostream& os);
	secure_vector<byte> encPrivateKey;
	std::vector<byte> publicKey;
	AutoSeeded_RNG aga;
	EC_Group faf = EC_Group("secp256k1");
	ECDSA_PrivateKey thePrivateKey = ECDSA_PrivateKey(aga, faf);
	secure_vector<byte> address;
	size_t sum = 0;
	Receiver receiver;
	std::shared_ptr<Blockchain> chain;
	std::vector<AddedOutput> availibleForAddress;

	serverd::server	m_server;
	serverd::miner 	m_miner;
};