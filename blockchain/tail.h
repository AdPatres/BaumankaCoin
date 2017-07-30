#pragma once

#ifndef TAIL_H
#define TAIL_H

#include <iostream>

#include <botan/secmem.h> // secure_vector

namespace ad_patres
{
  class Tail
  {
  public:
    Tail() = default;

    Tail(size_t, Botan::secure_vector<uint8_t>);

    ~Tail();

    bool
    operator==(const Tail& tail) const;

    bool setValue(size_t);

    bool setAddress(Botan::secure_vector<uint8_t>);

    bool
    scan(std::vector<uint8_t>, uint32_t&);

    std::pair<size_t, Botan::secure_vector<uint8_t>>
    getInfo() const;

    std::vector<uint8_t>
    convertTo8();

    friend std::ostream&
    operator<<(std::ostream& os, const Tail& o);

  private:
    size_t intValue = 0;
    Botan::secure_vector<uint8_t> address
      = Botan::secure_vector<uint8_t>(32, 0);
  };

  std::ostream&
  operator<<(std::ostream& os, const Tail& o);
} // namespace ad_patres

#endif // TAIL_H
