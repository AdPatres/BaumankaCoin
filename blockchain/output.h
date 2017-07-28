#pragma once

#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>

struct Output
{
  Output() = default;

  Output(size_t, size_t);

  bool
  operator!=(const Output&) const;

  bool
  operator==(const Output&) const;

  size_t blockNumber = 0;
  size_t txeNumber = 0;
};

std::ostream&
operator<<(std::ostream& os, const Output& o);

#endif // OUTPUT_H
