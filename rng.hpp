#ifndef RNG_HPP
#define RNG_HPP

#include <array>
#include <random>
#include <iostream>
#include <iterator>

/**
 * @brief The RNG class
 */
template<class Gen>
class RNG: public Gen {
public:
  /**
   * @brief RNG
   * @param sd
   */
  RNG(typename Gen::seed_type sd = Gen::default_seed): Gen(sd) {}

  /**
   * @brief RNG
   * @param rngst
   */
  RNG(typename Gen::state_type const& rngst): Gen(rngst) {}

  /**
   * @brief RNG
   * @param rngst
   */
  RNG(typename Gen::state_type && rngst): Gen(rngst) {}

public:
  /**
   * @brief getState
   * @return
   */
  typename Gen::state_type& getState() {
    return Gen::state;
  }

  /**
   * @brief setState
   * @param st
   */
  void setState(typename Gen::state_type const& st) {
    Gen::state = st;
  }

  /**
   * @brief setState
   * @param st
   */
  void setState(typename Gen::state_type && st) {
    Gen::state = st;
  }

private:
};

#endif // RNG_HPP
