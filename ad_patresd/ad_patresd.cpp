#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <memory>

#include "ad_patresd.hpp"

using namespace ad_patresd;

std::unique_ptr<server> g_server_ptr;

void
sighandler(int signum)
{
  switch (signum)
    {
      case SIGTERM:
      case SIGKILL:
        g_server_ptr->stop();
        g_server_ptr.release();
    }
}

pid_file::pid_file()
{
  m_pid_fd = open(g_pid_fname.c_str(), O_RDWR|O_CREAT, 0640);
  if (m_pid_fd < 0)
    throw std::runtime_error("Error");
  if (lockf(m_pid_fd, F_TLOCK, 0) == -1)
    throw std::runtime_error("Already running");
  std::string pid_str = std::to_string(getpid()) + '\n';
  write(m_pid_fd, pid_str.c_str(), pid_str.length());
}

pid_file::~pid_file()
{ close(m_pid_fd); }

void
start(unsigned short port)
{
  if (getppid() == 1) return;

  pid_t pid = fork();

  if (pid < 0)
    std::cerr << "error" << std::endl;
  if (pid > 0)
    return;

  umask(027);
  setsid();
  
  try
    {
      pid_file pf;
      signal(SIGKILL, sighandler);
      signal(SIGTERM, sighandler);
      g_server_ptr.reset(new server(port));
      g_server_ptr->start_accept();

    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
}

void
stop()
{
  std::ifstream pid_fs(g_pid_fname);
  pid_t pid;
  pid_fs >> pid;
  kill(-2, SIGTERM);
}
