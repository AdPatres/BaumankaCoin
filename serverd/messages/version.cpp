#include "version.hpp"

using namespace messages;

version::version()
{
  std::srand(std::time(0));
  m_nonce = static_cast<uint64_t>(std::rand());
}

std::ostream& operator<<(std::ostream& os, const version& obj)
{
  os.write(reinterpret_cast<const char*>(&obj.m_nonce), sizeof(obj.m_nonce));
  return os; 
}
