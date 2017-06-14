#include "fpid.hpp"

#include <fcntl.h> // open()
#include <signal.h> // kill()
#include <sys/stat.h> // umask()
#include <unistd.h> // getuid(), getppid(), setsid()

#include <fstream>
#include <memory> // unique_ptr
#include <string> // to_string()

namespace serverd
{
  fpid g_fpid;
}

using namespace serverd;

fpid::fpid()
: m_fname("/var/run/user/" + std::to_string(getuid()) + "/adpatresd.pid"),
  m_fd(::open(m_fname.c_str(), O_RDWR|O_CREAT, 0640))
{ }

fpid::~fpid()
{ ::close(m_fd); }

bool
fpid::write()
{
  // if (::lockf(m_fd, F_TLOCK, 0) == -1)
  //   return false;
  
  // std::string pid_str = std::to_string(getpid()) + '\n';
  // ::write(m_fd, pid_str.c_str(), pid_str.length());
  return true;
}

pid_t
fpid::read()
{
  std::fstream ifs(m_fname);
  pid_t pid;
  ifs >> pid;
  ifs.close();
  return pid;
}
