#include "./miner.hpp"

#include <iostream>

#include <botan/secmem.h>
#include <botan/sha2_32.h>

using namespace ad_patres;
using namespace Botan;

miner::miner(server* server_ptr) : m_server_ptr(server_ptr) {}
miner::~miner() { this->stop(); }
void
miner::start()
{
  state = true;
  m_mining_th.reset(new std::thread(&miner::m_mining, this));
  std::cerr << "miner started" << std::endl;
}

void
miner::stop()
{
  state = false;
  m_mining_th->join();
  if (m_mining_th.get())
    m_mining_th.release();
  std::cerr << "miner stopped" << std::endl;
}

void
miner::m_mining()
{
  while (state)
    {
      TransactionsMutex->lock_shared();
      Block block;
      block.addFirstTxe(m_local_address);
      size_t cnt = 0;
      if (Block::nonValidated.size() >= 4)
        {
          for (size_t i = 0; i < Block::nonValidated.size(); ++i)
            if (m_blockchain_ptr->validateTxn(
                  Block::nonValidated[i],
                  std::vector<std::pair<Output, size_t>>()))
              {
                block.addTransaction(i);
                if (++cnt == 4)
                  break;
              }
        }
      TransactionsMutex->unlock_shared();

      block.setMerkleRoot();

      BlockchainMutex->lock_shared();
      block.setNumber(m_blockchain_ptr->size());
      if (m_blockchain_ptr->size())
        block.setPrevBlock(m_blockchain_ptr->getLastBlockData());
      BlockchainMutex->unlock_shared();

      bool finished = false;
      while (!finished && state)
        {
          BlockchainMutex->lock_shared();
          block.setNumber(m_blockchain_ptr->size());
          if (m_blockchain_ptr->size())
            block.setPrevBlock(m_blockchain_ptr->getLastBlockData());
          BlockchainMutex->unlock_shared();

          secure_vector<byte> hash = SHA_256().process(block.getBlockData());
          for (auto i = 0; i < m_bits; i++)
            {
              if (hash[i] != 0)
                break;
              finished = true;
            }
        }
      if (finished)
        m_server_ptr->share(block);
    }
}

bool
miner::getState()
{
  return state;
}
