#include "wallet.h"

std::vector<AddedOutput> Transaction::availibleTxes;
std::vector<Transaction> Block::nonValidated;


using namespace Botan;

void Wallet::runMiner()
{

}
void Wallet::welcome()
{

	std::cout << "choose variants" << std::endl; 
	std::cout << "enter Wallet - print 1" <<std::endl;
	std::cout << "create new Wallet - print 2" << std::endl;
	char c = ' ';
	std::cin >> c;
	switch(c)
	{
		case('1'):
		{
			secure_vector<byte> priv;
			std::vector<byte> pub;
			for (size_t i = 0; i < 287; i++)
			{
				char info;
				std::cin >> info;
				priv.push_back(info);
			}
			for (size_t i = 0; i < 279; i++)
			{
				char info;
				std::cin >> info;
				pub.push_back(info);
			}
			*this = Wallet(priv, pub);
			commandProg();
			break;
		}
		case('2'):
		{
			commandProg();
			break;
		}
		default:
		{
			std::cout << "you did smth wrong try again" << std::endl;
			welcome();
			break;
		}

	}
}
void Wallet::commandProg()
{
	std::cout << "Current Sum: " << sum << std::endl;
	std::cout << "choose variants" << std::endl;
	std::cout << "refresh Data - print 1" << std::endl;
	std::cout << "create new Transaction - print 2" << std::endl;
	if (minerState)
	{
		std::cout << "stop miner - print 3" << std::endl;
	}
	else
	{
		std::cout << "run miner - print 3" << std::endl;
	}
	char c = ' ';
	std::cin >> c;
	switch (c)
	{
		case('1'):
		{
			setAvailibleForAdress();
			setCurrentSum();
			commandProg();
			break;
		}
		case('2'):
		{
			createTxe(std::cin,std::cout);
			commandProg();
			break;
		}
		case('3'):
		{
			runMiner();
			commandProg();
		}
		default:
		{
			std::cout << "you did smth wrong try again" << std::endl;
			welcome();
			break;
		}
	}
}
Wallet& Wallet::operator = (const Wallet& v2)
{
	encPrivateKey = v2.encPrivateKey;
	publicKey = v2.publicKey;
    faf = v2.faf;
	thePrivateKey =v2.thePrivateKey;
	address = v2.address;
}
void Wallet::setAvailibleForAdress()//CHANGED
{
	availibleForAddress.clear();
	for (auto i : Transaction::availibleTxes)
	{
		for (auto j = 0; j< chain->blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails.size(); j++)
			if (chain->blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails[j].getInfo().second == address && !i.usedTails[j])
				availibleForAddress.push_back(i);
	}
}
void Wallet::readInputs(std::istream& is, std::ostream& os)//CHANGED
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
		receiver.addInput(Output(blockNum, txeNum), tail, chain->blockChain[blockNum].txs[txeNum].getTxeData());
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
		size_t money = 0;
		os << "Now enter money to send on that addr";
		is >> money;
		receiver.addTail(Tail(money, addr));
		os << "add more tails? y-yes, else - no";
		is >> answer;
	}
}
void Wallet::createTxe(std::istream& is, std::ostream& os)//CHANGED
{
	receiver.clear();
	os << "First add Inputs\n";
	for (auto i : availibleForAddress)
	{
		for (auto j = 0; j < chain->blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails.size(); j++)
			if (chain->blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails[j].getInfo().second == address && !i.usedTails[j])
				os << "Block Number: " << i.output.blockNumber << " Txe Number: " << i.output.txeNumber << "Tail Number: " << j << " " << "Ammount of money: " << chain->blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails[j].getInfo().first << "\n";
	}
	readInputs(is, os);
	readTails(is, os);
	receiver.sign(thePrivateKey);
	auto tx =receiver.get();
	//broadcast tx;
}
void Wallet::setCurrentSum()//CHANGED
{
	for (auto i : availibleForAddress)
	{
		for (auto j = 0; j < chain->blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails.size(); j++)
			if (chain->blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails[j].getInfo().second == address && !i.usedTails[j])
				sum += chain->blockChain[i.output.blockNumber].txs[i.output.txeNumber].tails[j].getInfo().first;
	}
}
Wallet::Wallet()//CHANGED
{
	AutoSeeded_RNG rng;
	encPrivateKey = PKCS8::BER_encode(thePrivateKey);// rng, "default"
	publicKey = thePrivateKey.subject_public_key();
	address = SHA_256().process(publicKey);
	setAvailibleForAdress();
	setCurrentSum();
	receiver.Initialize(publicKey);
	chain = Blockchain::instance();
}

Wallet::Wallet(secure_vector<byte> priv, std::vector<byte> pub) : encPrivateKey(priv), publicKey(pub), thePrivateKey(AlgorithmIdentifier(), encPrivateKey)//add//CHANGED
{
	address = SHA_256().process(publicKey);
	receiver.Initialize(publicKey);
	chain = Blockchain::instance();
}


Wallet::~Wallet()
{
}

std::vector<secure_vector<byte>>
Wallet::getHashesAfter(uint64_t idx) const//CHANGED
{
	++idx;
	std::vector<secure_vector<byte>> res(chain->blockChain.size() - idx);
	size_t j = 0;
	for (size_t i = idx; i < chain->blockChain.size(); ++i, ++j)
		res[j] = SHA_256().process(chain->blockChain[i].getBlockData());
	return std::move(res);
}
