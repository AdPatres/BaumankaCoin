#pragma once

#include "./blockchain/blockchain.h"

#include <memory>
#include <thread>

namespace serverd
{
  class miner
  {
  public:
    ~miner();

    void 
    start();

    void 
    stop();
  
  private:
    void
    m_mining();

    std::shared_ptr<Blockchain>   m_blockchain_ptr = Blockchain::instance();
    std::unique_ptr<std::thread>  m_mining_th;
    secure_vector<byte>           m_local_address;
    uint64_t                      m_bits = 1;
  };
} // namespace serverd
