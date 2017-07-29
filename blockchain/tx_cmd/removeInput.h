#pragma once

#ifndef REMOVEINPUT_H
#define REMOVEINPUT_H

#include "./command.h"

namespace ad_patres 
{
  class RemoveInput : public Command
  {
  public:
    RemoveInput(Output, size_t, std::vector<uint8_t>);

    void
    Execute();

    void
    unExecute();

  private:
    Output output;
    size_t tailNum;
    std::vector<uint8_t> hash;
  };
}; // namespace ad_patres

#endif // REMOVEINPUT_H
