#include <cstdlib>

#include "network.hpp"


boost::asio::io_service ios;

int
main()
{
  network::server srv(ios);
  srv.start_accept();

  return EXIT_SUCCESS;
}
