#pragma once

#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <iostream>

namespace messages
{
  class version // version message
  {
  public:
    version();

    uint64_t m_nonce;
  };
} // namespace messages

std::ostream& operator<<(std::ostream& os, const messages::version& obj);