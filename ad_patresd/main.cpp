#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "ad_patresd.hpp"

int 
main(int argc, char* argv[]) 
{
  int opt = getopt(argc, argv, "ei");

  switch (opt)
    {
      case 'i':
        start();
        break;
      case 'e':
        stop();
        break;
      default:
        std::cerr << "Usage: " << argv[0] << " -[e|i]" << std::endl;
        exit(EXIT_FAILURE);
    }

  exit(EXIT_SUCCESS);
}
