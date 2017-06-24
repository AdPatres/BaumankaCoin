#include "miner.hpp"

using namespace serverd;

miner::miner(serverd::server* server_ptr)
: m_server_ptr(server_ptr)
{ }

miner::~miner()
{ this->stop(); }

void
miner::start()
{ 
	state = true;//NEW
  m_mining_th.reset(new std::thread(&miner::m_mining, this));
  m_mining_th->join();
}

void
miner::stop()
{ 
	state = false;//new
  if (m_mining_th.get()) 
    m_mining_th.release(); 
}

void
miner::m_mining()
{
  while (state) 
    {
      TransactionsMutex->lock_shared();//NEW
      Block block;
      block.addFirstTxe(m_local_address);
      size_t cnt = 0;
      for (size_t i = 0; i < Block::nonValidated.size(); ++i)
        if (m_blockchain_ptr->validateTxn(Block::nonValidated[i], std::vector<std::pair<Output, size_t>>()))
          {
            block.addTransaction(i);
            if (++cnt == 4) break;
          }
      TransactionsMutex->unlock_shared();//NEW
      block.setMerkleRoot();
      BlockchainMutex->lock_shared();//NEW
      block.setNumber(m_blockchain_ptr->size());
      block.setPrevBlock(m_blockchain_ptr->getLastBlockData());
      BlockchainMutex->unlock_shared();//NEW
      bool finished=false;
      while (!finished && !state)//NEW
        {
          BlockchainMutex->lock_shared();//NEW
          block.setNumber(m_blockchain_ptr->size());
          block.setPrevBlock(m_blockchain_ptr->getLastBlockData());
          BlockchainMutex->unlock_shared();//NEW
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
