#pragma once

#ifndef MINER_H
#define MINER_H

#include "./blockchain/blockchain.h"
#include "./network/server.hpp"

#include <memory> // shared_ptr, unique_ptr
#include <thread>

#include <botan/secmem.h>

namespace ad_patres
{
  class miner
  {
  public:
    miner(server*);

    ~miner();

    void
    start();

    void
    stop();

    bool
    getState();

  private:
    void
    m_mining();

    server* m_server_ptr;

    std::shared_ptr<Blockchain> m_blockchain_ptr = Blockchain::instance();
    std::unique_ptr<std::thread> m_mining_th;
    Botan::secure_vector<uint8_t> m_local_address;
    uint64_t m_bits = 1;
    bool state = false;
  };
} // namespace serverd

#endif // MINER_H
