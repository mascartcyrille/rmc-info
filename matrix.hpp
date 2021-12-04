#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <random>
#include <vector>
#include <algorithm>

#include "rng.hpp"

class Matrix: public std::vector<int> {
  public:
    Matrix(int size): std::vector<int>(size*size, 0), size(size) {}

  public:
    void initRandom(double threshold, RNG& prng) {
      std::generate(this->begin(), this->end(), [&](){return (prng.generate() <= threshold)? 1: 0;});
    }

private:
    int const size;
};

#endif // MATRIX_HPP
