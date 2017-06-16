#pragma once
#include "Input.h"
#include "tail.h"
#include <botan/ecdsa.h>
#include <botan/auto_rng.h>//rand numb gen
#include <botan/pubkey.h>//PK_SIGNER
class Blockchain;
class Wallet;

struct AddedOutput
{
	AddedOutput() = default;
	AddedOutput(Output out) :output(out){}
	AddedOutput(Output out, size_t tailsSize) : output(out), usedTails(tailsSize, false) {}
	Output output;
	std::vector<bool> usedTails;
};
class Transaction
{
	friend Blockchain;
	friend Wallet;
public:
	Transaction() = default;
	Transaction(std::vector<byte> pubKey);
	void clear();
	bool addInput(Output, size_t, std::vector<uint8_t>);
	bool removeInput(Output, size_t, std::vector<uint8_t>);
	bool addTail(Tail);
	bool removeTail(Tail);
	bool sign(ECDSA_PrivateKey);
	bool addAvailibleTxe(Output, size_t);
	bool scanBroadcastedData(std::vector<uint8_t> data, uint32_t& position);
	std::vector<uint8_t> getBroadcastData();
	std::vector<uint8_t> getTxeData() const;
	~Transaction();

protected:
	std::vector<Input> inputs;
	std::vector<Tail> tails;
	std::vector<byte> pubKey = std::vector<byte>(279, 0);
	std::vector<byte> signature = std::vector<byte>(64, 0);
	static std::vector<AddedOutput> availibleTxes;
};

