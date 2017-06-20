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
{ }
