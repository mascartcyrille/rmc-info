#ifndef RNG_HPP
#define RNG_HPP

#include <array>
#include <random>
#include <iostream>
#include <iterator>

template<class State>
struct RNGState: public State {
  RNGState() = default;
  RNGState(RNGState<State> const& st) = default;
  RNGState(RNGState<State> && st) = default;
  RNGState& operator=(RNGState const& st) = default;
  RNGState& operator=(RNGState && st) = default;
  ~RNGState() = default;
};

template<class Gen>
class RNG: public Gen {
public:
  RNG(typename Gen::seed_type sd = Gen::default_seed): Gen(sd) {}

  RNG(typename Gen::state_type const& rngst): Gen(rngst) {}

public:
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
