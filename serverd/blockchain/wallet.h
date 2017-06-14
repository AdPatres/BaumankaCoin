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
#include "blockchain.h"
#include <iostream>

using namespace Botan;

class Wallet
{
public:
	Wallet();
	Wallet(secure_vector<byte>, std::vector<byte>);
	void setAvailibleForAdress();
	void setCurrentSum();
	void createTxe(std::istream& is, std::ostream& os);
	secure_vector<byte> getLastBlockHash();
	~Wallet();
private:
	void readInputs(std::istream& is, std::ostream& os);
	void readTails(std::istream& is, std::ostream& os);
	secure_vector<byte> encPrivateKey;
	std::vector<byte> publicKey;
	AutoSeeded_RNG aga;
	EC_Group faf = EC_Group("secp256k1");
	ECDSA_PrivateKey thePrivateKey = ECDSA_PrivateKey(aga, faf);
	secure_vector<byte> address;
	size_t sum;
	Transaction current;
	Blockchain chain;
	std::vector<AddedOutput> availibleForAddress;
	
	
	//PKCS8* priv;
	//X509* pub;
};
