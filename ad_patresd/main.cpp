#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "ad_patresd.hpp"

int 
main(int argc, char* argv[]) 
{
  unsigned short port = 0;
  int opt = getopt(argc, argv, "9p:");

  switch (opt)
    {
      case 'p':
        port = static_cast<unsigned short>(std::strtoul(optarg, nullptr, 0));
        start(port);
        break;
      case '9':
        stop();
        break;
      default:
        std::cerr << "Usage: " << argv[0] << " -[9|p portnum]" << std::endl;
        exit(EXIT_FAILURE);
    }

  exit(EXIT_SUCCESS);
}
