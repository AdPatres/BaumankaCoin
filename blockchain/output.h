#pragma once

#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>

namespace ad_patres
{
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
  operator<<(std::ostream& os, const ad_patres::Output& o);
} // namespace ad_patres

#endif // OUTPUT_H
