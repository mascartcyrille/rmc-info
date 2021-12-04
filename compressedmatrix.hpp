#ifndef COMPRESSEDMATRIX_HPP
#define COMPRESSEDMATRIX_HPP

#include <array>
#include <vector>

class CompressedMatrix: public std::vector<std::array<int, 312>>
{
public:
  CompressedMatrix(): std::vector<std::array<int, 312>>() {}
};

#endif // COMPRESSEDMATRIX_HPP
