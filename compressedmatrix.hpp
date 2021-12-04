#ifndef COMPRESSEDMATRIX_HPP
#define COMPRESSEDMATRIX_HPP

#include <array>
#include <vector>

#include "rng.hpp"

class CompressedMatrix: public std::vector<std::pair<RNGState, bool>> {
public:
  CompressedMatrix(int sz): std::vector<std::pair<RNGState, bool>>(sz, std::pair<RNGState, bool>(RNGState(), false)), size(sz), nbr_non_null_elements(0) {}

  void setElement(size_t pos, RNGState state) {
    if((*this)[pos].second == false) {
      ++nbr_non_null_elements;
      (*this)[pos].second = true;
    }
    (*this)[pos].first = state;
  }

  bool full() {
    return nbr_non_null_elements==size;
  }

private:
  int size;
  int nbr_non_null_elements;
};

#endif // COMPRESSEDMATRIX_HPP
