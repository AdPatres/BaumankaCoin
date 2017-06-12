#pragma once
#include <botan/botan.h>
#include "transaction.h"
#include <string.h>//memset
#include <vector>
#include <stdlib.h>//itoa
#include <botan/sha2_32.h>

using namespace Botan;
class Block
{
	friend  Blockchain;
	friend Wallet;
public:
	Block() = default;
	bool addFirstTxe(secure_vector<byte> address);
	bool addTransaction(size_t = nonValidated.size());
	bool setPrevBlock(std::vector<uint8_t>);
	void calculate();
	void broadcastBlock();
	std::vector<uint8_t> getBlockData() const;
	std::vector<uint8_t> getTxeData(size_t) const; 
	bool setMerkleRoot();
	~Block();
protected:
	size_t version = 1;//default value
	secure_vector<byte> prevBlock;//hash
	size_t currentNumber=0;
	secure_vector<byte> merkleRoot;
	//timestamp
	size_t bits = 1;//Proof of work difficulty
	size_t nonce = 0;//to change hash
	size_t txsCount=0;
	//hash untill this
	std::vector<Transaction>  txs;
	//static pool for non validated txes
	static std::vector<Transaction> nonValidated;
	
};

