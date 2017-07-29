#include "./block.h"

#include <cassert>

using namespace ad_patres;
using namespace Botan;

bool
Block::addFirstTxe(secure_vector<byte> address)
{
  if (txs.size() != 0 || txsCount != 0)
    return false;
  Transaction first;
  Tail tail(100, address); // HARDCODED NUMBER
  first.addTail(tail);
  txs.push_back(first);
  txsCount++;
  return true;
}

uint32_t
Block::getCurrentNumber() const
{
  return currentNumber;
}

bool
Block::addTransaction(size_t num)
{
  if (num < nonValidated.size() && txs.size() < 8) // 8 txs in a block maximum
    {
      txs.push_back(nonValidated[num]);
      txs.erase(nonValidated.begin() + num);
      return true;
    }
  return false;
}

std::vector<uint8_t>
Block::getBlockData() const
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
  // std::memset(buffer, 0, sizeof(buffer));
}

void
Block::setNumber(size_t num)
{
  currentNumber = num;
}

bool
Block::setPrevBlock(std::vector<uint8_t> data)
{
  prevBlock = SHA_256().process(data);
}
void
Block::calculate() // should be modified to check if last block changed
{
}

std::vector<uint8_t>
Block::getTxeData(size_t num) const
{
  assert(num < txs.size()); // change for exceptions
  return txs[num].getTxeData();
}
bool
Block::setMerkleRoot()
{
  if (txs.size() == 1 || txs.size() == 2 || txs.size() == 4
      || txs.size() == 8) // HARDCODE
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
                  secure_vector<byte> tempData(hashes[i - 1]);
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
Block::~Block() {}
std::vector<uint8_t>
Block::getBroadcastData() const
{
  std::vector<uint8_t> data = getBlockData();
  for (auto txe : txs)
    {
      std::vector<uint8_t> info = txe.getBroadcastData();
      for (auto c : info)
        data.push_back(c);
    }
  return data;
}
void
Block::setHash(std::vector<byte> from, uint32_t& position,
               secure_vector<uint8_t>& to)
{
  to.clear();
  for (uint32_t i = 0; i < 32; i++)
    {
      to.push_back(from[position + i]);
    }
  position += 32;
}
bool
Block::scanBroadcastedData(
  std::vector<uint8_t> data,
  uint32_t& position) // make checks for amount of data;
{
  version = converter8to32(data, position);
  setHash(data, position, prevBlock);
  currentNumber = converter8to32(data, position);
  setHash(data, position, merkleRoot);
  bits = converter8to32(data, position);
  nonce = converter8to32(data, position);
  txsCount = converter8to32(data, position);
  for (uint32_t i = 0; i < txsCount; i++)
    {
      Transaction txn;
      txn.scanBroadcastedData(data, position);
      txs.push_back(txn);
    }
  return true;
}

void
Block::showInfo() const
{
  std::cerr << "Block: " << std::endl
            << "version: " << version << std::endl
            << "Current number: " << currentNumber << std::endl
            << "Bits: " << bits << std::endl
            << "Nonce: " << nonce << std::endl
            << "Transaction count: " << txsCount << std::endl
            << "Merkle root: " << Botan::hex_encode(merkleRoot) << std::endl;
  for (auto a : txs)
    a.showInfo();
}