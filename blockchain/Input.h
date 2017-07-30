#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "./output.h"

#include <iostream>
#include <vector>

#include <botan/secmem.h>

namespace ad_patres
{
  class Input
  {
  public:
    Input() = default;

    ~Input();

    void setOutput(size_t, size_t);

    void setOutput(Output);

    void setHash(std::vector<uint8_t>);

    void setTailNum(size_t);

    bool
    scan(std::vector<uint8_t>, uint32_t&);

    bool match(Output, size_t, std::vector<uint8_t>);

    std::pair<Output, size_t>
    getInfo() const;

    std::vector<uint8_t>
    convertTo8();

    bool
    operator==(const Input&);

    friend std::ostream&
    operator<<(std::ostream&, const Input&);

  private:
    Output output;
    Botan::secure_vector<uint8_t> outputHash
      = Botan::secure_vector<uint8_t>(32, 0);
    size_t tailNum = 0;
  };

  std::ostream&
  operator<<(std::ostream&, const ad_patres::Input&);
} // namespace ad_patres

void
converter32to8(uint32_t from, std::vector<uint8_t>& to);

uint32_t
converter8to32(std::vector<uint8_t>, uint32_t&);

#endif // INPUT_H
