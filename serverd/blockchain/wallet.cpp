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
// :thePrivateKey(AutoSeeded_RNG(), EC_Group("secp256k1"))
{
	AutoSeeded_RNG rng;
	encPrivateKey = PKCS8::BER_encode(thePrivateKey);// rng, "default"/* const std::string &password, const std::string &pbe_algo = ""*/);
	publicKey = thePrivateKey.subject_public_key();
	address = SHA_256().process(publicKey);
	setAvailibleForAdress();
	setCurrentSum();
	current = Transaction(publicKey);

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Public_Key* thePublicKey = X509::load_key(publicKey);
	//std::string text("This is a tasty burger!");
	//std::vector<uint8_t> data(text.data(), text.data() + text.length());
	// sign data
	//Botan::PK_Signer signer(thePrivateKey, rng, "EMSA1(SHA-256)");
	//signer.update(data);
	//std::vector<uint8_t> signature = signer.signature(rng);
	//std::cout << "Signature:" << std::endl << Botan::hex_encode(signature);
	// verify signature
	//Botan::PK_Verifier verifier(*thePublicKey, "EMSA1(SHA-256)");
	//verifier.update(data);
	//std::cout << std::endl << "is " << (verifier.check_signature(signature) ? "valid" : "invalid");
	//return 0;
}

Wallet::Wallet(secure_vector<byte> priv, std::vector<byte> pub) : encPrivateKey(priv), publicKey(pub), thePrivateKey(AlgorithmIdentifier(), encPrivateKey)//add
{
	address = SHA_256().process(publicKey);
	current = Transaction(publicKey);
}


Wallet::~Wallet()
{
}
