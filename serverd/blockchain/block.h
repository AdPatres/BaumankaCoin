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
	std::vector<uint8_t> getBroadcastData();
	bool scanBroadcastedData(std::vector<uint8_t>, uint32_t&);
protected:
	void setHash(std::vector<byte> from, uint32_t& position,
		secure_vector<uint8_t>& to);
	size_t version = 1;//default value
	secure_vector<byte> prevBlock = secure_vector<byte>(32,0);//hash
	size_t currentNumber=0;
	secure_vector<byte> merkleRoot = secure_vector<byte>(32,0);;
	//timestamp
	size_t bits = 1;//Proof of work difficulty
	size_t nonce = 0;//to change hash
	size_t txsCount=0;
	//hash untill this
	std::vector<Transaction>  txs;
	//static pool for non validated txes
	static std::vector<Transaction> nonValidated;
	
};

