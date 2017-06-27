#include "transaction.h"

std::shared_ptr<std::shared_timed_mutex> BlockchainMutex(new std::shared_timed_mutex);//NEW
std::shared_ptr<std::shared_timed_mutex> TransactionsMutex(new std::shared_timed_mutex);//NEW


Transaction::Transaction(std::vector<byte> pub):pubKey(pub)
{
}
void Transaction::clear()
{
	inputs.clear();
	tails.clear();
	signature.clear();
}
bool Transaction::operator==(Transaction& txe)//NEW
{
	/*std::vector<Input> inputs;
	std::vector<Tail> tails;
	std::vector<byte> pubKey = std::vector<byte>(279, 0);
	std::vector<byte> signature = std::vector<byte>(64, 0);*/
	if (inputs.size() != txe.inputs.size() || tails.size() != txe.tails.size())
		return false;
	for (size_t i = 0; i < inputs.size(); ++i)
	{
		if (!(inputs[i] == txe.inputs[i]))
			return false;
	}
	for (size_t i = 0; i < tails.size(); ++i)
	{
		if (!(tails[i]==tails[i]))
			return false;
	}
	if (pubKey != txe.pubKey)
		return false;
	if (signature != txe.signature)
		return false;
	return true;
}
bool Transaction::addInput(Output output, size_t tail, std::vector<uint8_t> info)
{
	Input data;
	data.setOutput(output);
	data.setTailNum(tail);
	data.setHash(info);
	inputs.push_back(data);
	return true;
}
bool Transaction::removeInput(Output output, size_t tail, std::vector<uint8_t> info)
{
	
	for (auto i=0; i<inputs.size();i++)
	{
		if (inputs[i].match(output, tail, info))
		{
			inputs.erase(inputs.begin() + i);
			return true;
		}
	}
	return false;
}
bool Transaction::addTail(Tail tail)
{
	tails.push_back(tail);
	return true;
}
bool Transaction::removeTail(Tail tail)
{
	for (auto i = 1; i < tails.size(); i++)
	{
		if (tails[i] == tail)
		{
			tails.erase(tails.begin() + i);
			return true;
		}
		return false;
	}
}
bool Transaction::addAvailibleTxe(Output output, size_t tailsSize)
{
	availibleTxes.push_back(AddedOutput(output, tailsSize));
	return true;
}

std::vector<uint8_t> Transaction::getTxeData() const
{
	std::vector<uint8_t> info;
	for (auto i : inputs)
	{
		std::vector<uint8_t> data = i.convertTo8();
		for (auto j : data)
		{
			info.push_back(j);
		}
	}
	for (auto i : tails)
	{
		std::vector<uint8_t> data = i.convertTo8();
		for (auto j : data)
		{
			info.push_back(j);
		}
	}
	for (auto i : pubKey)
	{
		info.push_back(i);
	}
	for (auto i : signature)
	{
		info.push_back(i);
	}
	return info;
}

bool Transaction::sign(ECDSA_PrivateKey key)
{
	AutoSeeded_RNG rng;
	std::vector<uint8_t> data = this->getTxeData();
	PK_Signer signer(key, rng, "EMSA1(SHA-256)");
	signer.update(data);
	signature = signer.signature(rng);
}

Transaction::~Transaction()
{
}

std::vector<uint8_t> Transaction::getBroadcastData() const
{
	uint32_t inputsAmount= inputs.size();
	uint32_t tailsAmount = tails.size();
	std::vector<uint8_t> data;
	converter32to8(inputsAmount, data);
	converter32to8(tailsAmount, data);
	std::vector<uint8_t> info = getTxeData();
	for (auto c : info)
		data.push_back(c);
	return data;
}
bool Transaction::scanBroadcastedData(std::vector<uint8_t> data, uint32_t& position)
{
	uint32_t inputsAmount = converter8to32(data, position);
	uint32_t tailsAmout = converter8to32(data, position);
	for (uint32_t i = 0; i < inputsAmount; i++)
	{
		Input input;
		input.scan(data, position);
		inputs.push_back(input);
	}
	for (uint32_t i = 0; i < tailsAmout; i++)
	{
		Tail tail;
		tail.scan(data, position);
		tails.push_back(tail);
	}
	pubKey.clear();
	for (uint32_t i = 0; i < 279; i++) // TODO: check size
	{
		pubKey.push_back(data[position]);
		position++;
	}
	signature.clear();
	for (uint32_t i = 0; i < 64; i++) // TODO: check size
	{
		signature.push_back(data[position]);
		position++;
	}
	return true;
}

bool 
Transaction::removeSign()
{
	signature = std::vector<byte>(64, 0);
	return true;
}

void 
Transaction::showInfo() const 
{
	std::cerr << "Transaction:"	<< std::endl;
	std::cerr << "Pubkey " 			<< Botan::hex_encode(pubKey) << std::endl;
	std::cerr << "Signature " 	<< Botan::hex_encode(signature) << std::endl;
	for (auto i : inputs)
		std::cerr << i;
	for (auto i : tails)
		std::cerr << i;
}
