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
    template<class Gen>
    void initRandom(typename Gen::result_type threshold, RNG<Gen>& prng) {
      std::generate(this->begin(), this->end(), [&](){return (prng.generate() <= threshold)? 1: 0;});
    }

    template<class Gen>
    void initFromCompressed(CompressedMatrix<Gen> cm) {
      int start = 0 ;
      int end = size - 1 ;
      for(int line = 0; line < size; ++line) {
        if(std::get<1>(cm[line]) == true) {
          RNG<Gen> prng(std::get<0>(cm[line]));
          for(int col = start ; col <= end ; col++)
          {
            (*this)[col] = (prng.generate() <= std::get<2>(cm[line]))? 1: 0 ;
          }
          start = start + size ;
          end = end + size ;
      //    auto start = this->begin() + line;
       //   std::generate(start, start + size, [&](){return (prng.generate() <= threshold)? 1: 0;});
        }
      }
    }

private:
    int const size;
};

#endif // MATRIX_HPP
