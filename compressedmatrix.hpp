#ifndef COMPRESSEDMATRIX_HPP
#define COMPRESSEDMATRIX_HPP

#include <array>
#include <vector>
#include <tuple>

#include "rng.hpp"

/**
 * @brief The CompressedMatrix class
 */
template<class Gen>
class CompressedMatrix: public std::vector<std::tuple<typename Gen::state_type, bool, double>> {
public:
  /**
   * @brief CompressedMatrix
   * @param sz
   */
  CompressedMatrix(int sz)
    : std::vector<std::tuple<typename Gen::state_type, bool, double>>(sz, std::tuple<typename Gen::state_type, bool, double>(typename Gen::state_type(), false, 0)),
      size(sz),
      nbr_non_null_elements(0)
  {

  }

  void setElement(size_t pos, typename Gen::state_type state, double treshold)
  {
    if(std::get<1>((*this)[pos]) == false) {
      ++nbr_non_null_elements;
      std::get<1>((*this)[pos]) = true;
    }
    std::get<0>((*this)[pos]) = state;
    std::get<2>((*this)[pos]) = treshold;
  }

  /**
   * @brief full
   * @return
   */
  bool full() {
    return nbr_non_null_elements==size;
  }

private:
  /**
   * @brief size
   */
  int size;
  /**
   * @brief nbr_non_null_elements
   */
  int nbr_non_null_elements;
};

#endif // COMPRESSEDMATRIX_HPP
