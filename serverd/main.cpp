#include <unistd.h>

#include "fpid.hpp"
#include "server.hpp"

#include <fcntl.h> // open()
#include <signal.h> // kill()
#include <sys/stat.h> // umask()
#include <unistd.h> // getuid(), getppid(), setsid()

#include <cstdlib>
#include <cstdint> // uint16_t
#include <exception>
#include <memory> // unique_ptr
#include <iostream>

void
start(uint16_t port);

void
stop();


// std::vector<AddedOutput> Transaction::availibleTxes;
// std::vector<Transaction> Block::nonValidated;

int
main(int argc, char* argv[]) try
{
  uint16_t port = 0;
  int opt = getopt(argc, argv, "9p:");

  switch (opt)
    {
      case 'p':
        port = static_cast<uint16_t>(std::strtoul(optarg, nullptr, 0));
        start(port);
        break;
      case '9':
        stop();
        break;
      default:
        std::cerr << "Usage: " << argv[0] << " -[9|p portnum]" << std::endl;
        exit(EXIT_FAILURE);
    }

  return EXIT_SUCCESS;
}
catch (std::exception& e)
{ std::cerr << e.what() << std::endl; }

void
start(uint16_t port)
{
  if (getppid() == 1) 
    {
      std::cerr << "already demonized" << std::endl;
      return;
    }

  pid_t pid = fork();
  if (pid != 0) return; // exit from non-demon

  umask(027);
  setsid();
  if (!serverd::g_fpid.write()) 
    {
      std::cerr << "already running" << std::endl;
      return;
    }

  serverd::g_server_ptr.reset(new serverd::server(port));
  serverd::g_server_ptr->start();
  serverd::g_server_ptr->accept();
}

void
stop()
{
  kill(serverd::g_fpid.read(), SIGKILL);
}
