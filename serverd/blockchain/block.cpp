#include "block.h"




bool Block::addFirstTxe(secure_vector<byte> address)
{
	if (txs.size() != 0 || txsCount != 0)
		return false;
	Transaction first;
	Tail tail(100, address);//HARDCODED NUMBER
	first.addTail(tail);
	txs.push_back(first);
	txsCount++;
	return true;
}

bool Block::addTransaction(size_t num)
{
	if (num < nonValidated.size()&& txs.size()<8)//8 txs in a block maximum
	{
		txs.push_back(nonValidated[num]);
		txs.erase(nonValidated.begin() + num);
		return true;
	}
	return false;
}

std::vector<uint8_t> Block::getBlockData() const
{
	std::vector<uint8_t> data;
	converter32to8(version, data);
	for (auto i : prevBlock)
	{
		data.push_back(i);
	}
	converter32to8(currentNumber, data);
	for (auto i : merkleRoot)
	{
		data.push_back(i);
	}
	converter32to8(bits, data);
	converter32to8(nonce, data);
	converter32to8(txsCount, data);
	
	return data;
	//std::memset(buffer, 0, sizeof(buffer));
}

bool Block::setPrevBlock(std::vector<uint8_t> data)
{
	prevBlock = SHA_256().process(data);
}
void Block::calculate()//should be modified to check if last block changed
{
	bool finished=false;
	while (!finished)
	{
		secure_vector<byte> hash = SHA_256().process(getBlockData());
		for (auto i = 0; i < bits; i++)
		{
			if (hash[i] != 0)
				break;
		}
		finished = true;
	}
}

std::vector<uint8_t> Block::getTxeData(size_t num) const
{
	assert(num < txs.size());//change for exceptions
	return txs[num].getTxeData();
}
bool Block::setMerkleRoot()
{
	if (txs.size() == 1 || txs.size() == 2 || txs.size() == 4 || txs.size() == 8)//HARDCODE
	{
		std::vector<secure_vector<byte>> hashes;
		for (auto i = 0; i < txs.size(); i++)
		{
			hashes.push_back(SHA_256().process(txs[i].getTxeData()));
		}
		while (hashes.size() != 1)
		{
			std::vector<secure_vector<byte>> tempHashes;
			for (auto i = 0; i < hashes.size(); i++)
			{
				if (i % 2 == 1)
				{
					secure_vector<byte> tempData(hashes[i-1]);
					for (auto j : hashes[i])
					{
						tempData.push_back(j);
					}
					tempHashes.push_back(SHA_256().process(tempData));
				}
				hashes = tempHashes;
			}
		}
		merkleRoot = hashes[0];
		return true;
	}
	return false;
}
Block::~Block()
{
}
