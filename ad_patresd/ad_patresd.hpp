#pragma once

#include <unistd.h>

#include <exception>
#include <string>

#include "server.hpp"

namespace ad_patresd
{
  const std::string g_pid_fname = "/var/run/user/" + std::to_string(getuid()) 
                                  + "/ad_patresd.pid";

  class pid_file {
  public:
    pid_file();

    ~pid_file();

  private:
    int m_pid_fd;
  };
} // ad_patres

void
start(unsigned short);

void 
stop();
