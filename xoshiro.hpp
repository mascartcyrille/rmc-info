#ifndef XOSHIRO_HPP
#define XOSHIRO_HPP

#include <cstdint>

struct Xoshiro256State {
  typedef uint64_t result_type;

  result_type s[4];
};

class Xoshiro256 {
public:
  using state_type = Xoshiro256State;
  using result_type = state_type::result_type;

public:
  /* This is xoshiro256++ 1.0, one of our all-purpose, rock-solid generators.
     It has excellent (sub-ns) speed, a state (256 bits) that is large
     enough for any parallel application, and it passes all tests we are
     aware of.

     For generating just floating-point numbers, xoshiro256+ is even faster.

     The state must be seeded so that it is not everywhere zero. If you have
     a 64-bit seed, we suggest to seed a splitmix64 generator and use its
     output to fill s. */

  result_type generate(void) {
    result_type const result = rotl(state.s[0] + state.s[3], 23) + state.s[0];

    { // Update the state
      result_type const t = state.s[1] << 17;

      state.s[2] ^= state.s[0];
      state.s[3] ^= state.s[1];
      state.s[1] ^= state.s[2];
      state.s[0] ^= state.s[3];

      state.s[2] ^= t;

      state.s[3] = rotl(state.s[3], 45);
    }

    return result;
  }

private:
  inline constexpr result_type rotl(result_type const x, int k) {
    return (x << k) | (x >> (64 - k));
  }

private:
  state_type state;
};

#endif // XOSHIRO_HPP
