#pragma once

#ifndef MINER_H
#define MINER_H

#include "./blockchain/blockchain.h"
#include "./network/server.hpp"

#include <memory> // shared_ptr, unique_ptr
#include <thread>

namespace serverd
{
  class miner
  {
  public:
    miner(serverd::server*);

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

    serverd::server* m_server_ptr;

    std::shared_ptr<Blockchain> m_blockchain_ptr = Blockchain::instance();
    std::unique_ptr<std::thread> m_mining_th;
    secure_vector<byte> m_local_address;
    uint64_t m_bits = 1;
    bool state = false;
  };
} // namespace serverd

#endif // MINER_H
