#pragma once

#include <cstdint> // uint16_t
#include <memory> // unique_ptr

namespace serverd
{
  class fpid // pid file RAII class
  {
    // TODO: errors
  public:
    fpid();

    ~fpid();

    bool
    write(); // write pid to file

    pid_t
    read(); // read pid from file

  private:
    const std::string m_fname; 
    int m_fd;
  };
  extern fpid g_fpid;
} // namespace serverd
