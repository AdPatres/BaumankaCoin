#pragma once

#ifndef ADDINPUT_H
#define ADDINPUT_H

#include "./command.h"

#include <vector>

namespace ad_patres
{
  class AddInput : public Command
  {
  public:
    AddInput(Output out, size_t tail, std::vector<uint8_t>);
    
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

#endif // ADDINPUT_H
