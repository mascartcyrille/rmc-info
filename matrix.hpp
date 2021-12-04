#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <random>
#include <vector>
#include <iterator>
#include <algorithm>

#include "rng.hpp"
#include "compressedmatrix.hpp"

class Matrix: public std::vector<int> {
  public:
    Matrix(int size): std::vector<int>(size*size, 0), size(size) {}

  public:
    void initRandom(double threshold, RNG& prng) {
      std::generate(this->begin(), this->end(), [&](){return (prng.generate() <= threshold)? 1: 0;});
    }

    void initFromCompressed(CompressedMatrix cm, double threshold) {
      for(int line = 0; line < size; ++line) {
        if(cm[line].second == true) {
          RNG prng(cm[line].first);

          auto start = this->begin() + line;
          std::generate(start, start + size, [&](){return (prng.generate() <= threshold)? 1: 0;});
        }
      }
    }

private:
    int const size;
};

#endif // MATRIX_HPP
