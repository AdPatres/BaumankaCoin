#pragma once

#ifndef TAIL_H
#define TAIL_H

#include <iostream>

#include <botan/botan.h>

using namespace Botan;
class Tail
{
public:
  Tail() = default;

  Tail(size_t, secure_vector<byte>);

  ~Tail();

  bool
  operator==(const Tail& tail) const;

  bool setValue(size_t);

  bool setAddress(secure_vector<byte>);

  bool
  scan(std::vector<uint8_t>, uint32_t&);

  std::pair<size_t, secure_vector<byte>>
  getInfo() const;

  std::vector<uint8_t>
  convertTo8();

  friend std::ostream&
  operator<<(std::ostream& os, const Tail& o);

private:
  size_t intValue = 0;
  secure_vector<byte> address = secure_vector<uint8_t>(32, 0);
};

std::ostream&
operator<<(std::ostream& os, const Tail& o);

#endif // TAIL_H
