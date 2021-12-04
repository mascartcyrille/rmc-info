#ifndef COMPRESSEDMATRIX_HPP
#define COMPRESSEDMATRIX_HPP

#include <array>
#include <vector>

#include "rng.hpp"

class CompressedMatrix: public std::vector<RNGState>
{
public:
  CompressedMatrix(): std::vector<RNGState>() {}
};

#endif // COMPRESSEDMATRIX_HPP
