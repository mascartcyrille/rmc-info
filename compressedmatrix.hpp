#ifndef COMPRESSEDMATRIX_HPP
#define COMPRESSEDMATRIX_HPP

#include <array>
#include <vector>

#include "rng.hpp"

class CompressedMatrix: public std::vector<std::tuple<RNGState, bool, double>> {
public:
  CompressedMatrix(int sz): std::vector<std::tuple<RNGState, bool, double>>(sz, std::tuple<RNGState, bool, double>(RNGState(), false, 0)), size(sz), nbr_non_null_elements(0) {}

  void setElement(size_t pos, RNGState state, double treshold) {
    if(std::get<1>((*this)[pos]) == false) {
      ++nbr_non_null_elements;
      std::get<1>((*this)[pos]) = true;
    }
    std::get<0>((*this)[pos]) = state;
    std::get<2>((*this)[pos]) = treshold;
  }

  bool full() {
    return nbr_non_null_elements==size;
  }

private:
  int size;
  int nbr_non_null_elements;
};

#endif // COMPRESSEDMATRIX_HPP
