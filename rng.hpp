#ifndef RNG_HPP
#define RNG_HPP

#include <array>
#include <random>
#include <iostream>
#include <iterator>

template<class State>
struct RNGState: public State {};

template<class Gen>
class RNG: public Gen {

  ~RNG() = default;

  RNGState<typename Gen::state_type> getState() {
    return state;
  }

  void setState(RNGState<typename Gen::state_type>& st) {
    state = st;
  }

private:
  RNGState<typename Gen::state_type> state;
};



#endif // RNG_HPP
