#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <random>
#include <vector>
#include <iterator>
#include <algorithm>

#include "rng.hpp"
#include "compressedmatrix.hpp"

class Matrix : public std::vector<int>
{
public:
  Matrix(int size) : std::vector<int>(size * size, 0), size(size), thresholds(size, 0) {}

public:
  void initRandom(double threshold, RNG &prng)
  {
    std::generate(this->begin(), this->end(), [&]()
                  { return (prng.generate() <= threshold) ? 1 : 0; });
    int start = 0, end = size - 1;
    for (int i = 0; i < size; i++)
    {
      thresholds[i] = computeAverageThreshold(start, end) ;
      start = start + size;
      end = end + size;
    }
  }

  void initFromCompressed(CompressedMatrix cm)
  {
    int start = 0;
    int end = size - 1;
    for (int line = 0; line < size; ++line)
    {
      if (std::get<1>(cm[line]) == true)
      {
        RNG prng(std::get<0>(cm[line]));
        for (int col = start; col <= end; col++)
        {
          (*this)[col] = (prng.generate() <= std::get<2>(cm[line])) ? 1 : 0;
        }
        thresholds[line] = std::get<2>(cm[line]) ;
        //    auto start = this->begin() + line;
        //   std::generate(start, start + size, [&](){return (prng.generate() <= threshold)? 1: 0;});
      }
      start = start + size;
      end = end + size;
    }
  }

  // Compute average threshold
  double computeAverageThreshold(int start, int end)
  {
    double sum = 0;
    for (int i = start; i <= end; i++)
    {
      sum = sum + (*this)[i];
    }
    return sum / size;
  }

  // Getter avg treshold
  double getAverageThreshold(int line)
  {
    return thresholds[line] ;
  }

private:
  int const size;
  std::vector<double> thresholds;
};

#endif // MATRIX_HPP
