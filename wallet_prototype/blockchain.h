#pragma once
#include <vector>
#include "block.h"


class Blockchain
{
	friend Wallet;
public:
	Blockchain();
	void getBlockchain();
	void getNonValidatedTxes();
	bool validateBlockChain();
	bool addBlock(Block&);
	~Blockchain();
private:
	size_t bits = 1;
	bool validateBlock(Block&);
	bool validateMerkleRoot(const Block&);
	bool validateTxn(const Transaction&, std::vector<std::pair<Output, size_t>>);
	bool validateFirstTxn(const Transaction& );
	size_t validateInputs(const Transaction&, std::vector<std::pair<Output, size_t>>);//returns amount of money taken from inputs
	bool validateSignature(const Transaction&);
	bool validateTails(const Transaction&, size_t);
	void restore(std::vector<std::pair<Output, size_t>>);
	void setAvailibleTxes(Block&);
	void clearAvailibleTxes();
	std::vector<Block> blockChain;
	std::vector<Block> nonValidatedBlockChain;
};

