#include "wallet.h"

std::vector<AddedOutput> Transaction::availibleTxes;
std::vector<Transaction> Block::nonValidated;

using namespace Botan;

void Wallet::setAvailibleForAdress()
{
	availibleForAddress.clear();
	for (auto i : Transaction::availibleTxes)
	{
		for (auto j=0;j< chain.blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails.size();j++)
			if (chain.blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails[j].getInfo().second == address && !i.usedTails[j])
				availibleForAddress.push_back(i);
	}
}
void Wallet::readInputs(std::istream& is, std::ostream& os)
{
	uint8_t answer = 'y';
	while (answer == 'y')
	{
		size_t blockNum = 0;
		size_t txeNum = 0;
		size_t tail = 0;
		os << "Enter block num \n";
		is >> blockNum;
		os << "Enter txe num \n";
		is >> txeNum;
		os << "Enter tail num \n";
		is >> tail;
		current.addInput(Output(blockNum, txeNum), tail, chain.blockChain[blockNum].txs[txeNum].getTxeData());
		os << "add more inputs? y-yes, else - no";
		is >> answer;
	}
}
void Wallet::readTails(std::istream& is, std::ostream& os)
{
	os << "Now enter tails\n";
	uint8_t answer = 'y';
	while (answer == 'y')
	{
		os << "First enter addr\n";
		secure_vector<byte> addr;
		uint8_t input;
		while (input != '\n')
		{
			is >> input;
			addr.push_back(input);
		}
		size_t money=0;
		os << "Now enter money to send on that addr";
		is >> money;
		current.addTail(Tail(money, addr));
		os << "add more tails? y-yes, else - no";
		is >> answer;
	}
}
void Wallet::createTxe(std::istream& is, std::ostream& os)
{
	current.clear();
	os << "First add Inputs\n";
	for (auto i : availibleForAddress)
	{
		for (auto j = 0; j < chain.blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails.size(); j++)
			if (chain.blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails[j].getInfo().second == address && !i.usedTails[j])
				os << "Block Number: " << i.output.blockNumber << " Txe Number: " << i.output.txeNumber<<"Tail Number: "<<j<<" "<< "Ammount of money: " << chain.blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails[j].getInfo().first << "\n";
	}
	readInputs(is, os);
	readTails(is, os);
	current.sign(thePrivateKey);
}
void Wallet::setCurrentSum()
{
	for (auto i : availibleForAddress)
	{
		for (auto j = 0; j < chain.blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails.size(); j++)
			if (chain.blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails[j].getInfo().second == address && !i.usedTails[j])
				sum += chain.blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails[j].getInfo().first;
	}
}
Wallet::Wallet()
{
	AutoSeeded_RNG rng;
	encPrivateKey = PKCS8::BER_encode(thePrivateKey);// rng, "default"
	publicKey = thePrivateKey.subject_public_key();
	address = SHA_256().process(publicKey);
	setAvailibleForAdress();
	setCurrentSum();
	current = Transaction(publicKey);
	receiver.Initialize(publicKey);
}

Wallet::Wallet(secure_vector<byte> priv, std::vector<byte> pub) : encPrivateKey(priv), publicKey(pub), thePrivateKey(AlgorithmIdentifier(), encPrivateKey)//add
{
	address = SHA_256().process(publicKey);
	current = Transaction(publicKey);
	receiver.Initialize(publicKey);
}


Wallet::~Wallet()
{
}

secure_vector<byte>
Wallet::getLastBlockHash() // TODO: mutex
{
	return !chain.blockChain.empty()
		? SHA_256().process(chain.blockChain.back().getBlockData()) 
		: SHA_256().process(Block().getBlockData());
} 

int64_t 
Wallet::findByHash(secure_vector<byte> hash)
{
	for (int64_t i = 0; i < chain.blockChain.size(); ++i)
		{
			if (SHA_256().process(chain.blockChain[i].getBlockData()) == hash)
				return i;
		}
	return -1;
}

std::vector<secure_vector<byte>> 
Wallet::getHashesAfter(uint64_t idx) const
{
	++idx;
	std::vector<secure_vector<byte>> res(chain.blockChain.size() - idx);
	size_t j = 0;
	for (size_t i = idx; i < chain.blockChain.size(); ++i, ++j)
		res[j] = SHA_256().process(chain.blockChain[i].getBlockData());
	return std::move(res);
}

std::vector<Block> 
Wallet::getBlocksAfter(uint64_t idx) const
{
	++idx;
	std::vector<Block> res(chain.blockChain.size() - idx);
	for (size_t i = idx, j = 0; i < chain.blockChain.size(); ++i, ++j)
		res[j] = chain.blockChain[i];
	return std::move(res);
}

uint32_t
Wallet::getBlockchainSize() const
{	return chain.blockChain.size(); }
