#include "miner.hpp"

using namespace serverd;

miner::~miner()
{ this->stop(); }

void
miner::start()
{ 
  m_mining_th.reset(new std::thread(&miner::m_mining, this));
  m_mining_th->join();
}

void
miner::stop()
{ 
  if (m_mining_th.get()) 
    m_mining_th.release(); 
}

void
miner::m_mining()
{ 
  Block block;
  block.addFirstTxe(m_local_address);
  size_t cnt = 0;
  for (size_t i = 0; i < Block::nonValidated.size(); ++i)
    if (m_blockchain_ptr->validateTxn(Block::nonValidated[i], std::vector<std::pair<Output, size_t>>()))
      {
        block.addTransaction(i);
        if (++cnt == 4) break;
      }
  block.setMerkleRoot();
  block.setNumber(m_blockchain_ptr->size());
  block.setPrevBlock(m_blockchain_ptr->getLastBlockData());
  bool finished=false;
	while (!finished)
    {
      block.setNumber(m_blockchain_ptr->size());
      block.setPrevBlock(m_blockchain_ptr->getLastBlockData());
      secure_vector<byte> hash = SHA_256().process(block.getBlockData());
      for (auto i = 0; i < m_bits; i++)
        {
          if (hash[i] != 0)
            break;
          finished = true;
        }
    }
}
