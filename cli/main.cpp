#include <cstdlib>
#include <iostream>
#include <string>

bool 
cmd_proc(std::string&);

void 
startd();

int 
main() 
{
  std::string cmd;

  do
    {
      std::cout << "AdPatres > ";
      std::cin >> cmd;
    } 
  while (cmd_proc(cmd));

  return EXIT_SUCCESS;
}

bool
cmd_proc(std::string& cmd)
{
  if (cmd == "quit")
    {
        std::cout << "quitting";
        return false;
    }
  else
    std::cout << "unknown command" << std::endl;
  return true;
}
