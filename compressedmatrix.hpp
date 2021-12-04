#ifndef COMPRESSEDMATRIX_HPP
#define COMPRESSEDMATRIX_HPP

#include <array>
#include <vector>

#include "rng.hpp"

class CompressedMatrix: public std::vector<RNGState*> {
public:
  CompressedMatrix(int sz): std::vector<RNGState*>(sz, nullptr), size(sz), nbr_non_null_elements(0) {}

  void setElement(size_t pos, RNGState* state) {
    if(state != nullptr) {
      ++nbr_non_null_elements;
      (*this)[pos] = state;
    }
  }

  bool full() {
    return nbr_non_null_elements==size;
  }

private:
  int size;
  int nbr_non_null_elements;
};

#endif // COMPRESSEDMATRIX_HPP
