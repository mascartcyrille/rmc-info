#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <random>
#include <vector>
#include <iterator>
#include <algorithm>

#include "rng.hpp"
#include "compressedmatrix.hpp"

/**
 * @brief The Matrix class
 */
class Matrix: public std::vector<int> {
public:
  /**
   * @brief Matrix
   * @param size
   */
  Matrix(int sz)
    : std::vector<int>(sz * sz, 0),
      size(sz),
      thresholds(sz, 0)
  {

  }

public:
  /**
   * @brief initRandom
   * @param threshold
   * @param prng
   */
  template<class Gen>
  void initRandom(typename Gen::result_type const threshold, RNG<Gen>& prng)
  {
    std::generate(this->begin(),
                  this->end(),
                  [&](){ return (prng.generate() <= threshold) ? 1 : 0; }
    );
    int start = 0, end = size - 1;
    for (int i = 0; i < size; i++)
    {
      thresholds[i] = computeAverageThreshold(start, end) ;
      start = start + size;
      end = end + size;
    }
  }

  /**
   * @brief initFromCompressed
   * @param cm
   */
  template<class Gen>
  void initFromCompressed(CompressedMatrix<Gen> const& cm)
  {
    int start = 0 ;
    int end = size - 1 ;
    for(int line = 0; line < size; ++line)
    {
      if(std::get<1>(cm[line]) == true)
      {
        RNG<Gen> prng(std::get<0>(cm[line]));
        for(int col = start ; col <= end ; col++)
        {
          auto tmp = prng.generate();
          auto thr = std::get<2>(cm[line]);
          (*this)[col] = ((tmp <= thr)? 1: 0);
        }
      }
      start = start + size ;
      end = end + size ;
      thresholds[line] = std::get<2>(cm[line]) ;
    }
  }

  /**
   * @brief Compute average threshold
   * @param start
   * @param end
   * @return
   */
  double computeAverageThreshold(int start, int end) const
  {
    double sum = 0;
    for (int i = start; i <= end; i++)
    {
      sum = sum + (*this)[i];
    }
    return sum / size;
  }

  /**
   * @brief Getter avg treshold
   * @param line
   * @return
   */
  double getAverageThreshold(int line) const
  {
    return thresholds[line] ;
  }

private:
  /**
   * @brief size
   */
  int const size;
  /**
   * @brief thresholds
   */
  std::vector<double> thresholds;
};

#endif // MATRIX_HPP
