#ifndef MWC_HPP
#define MWC_HPP

#include <cstdint>

struct MWC128State {
  typedef uint64_t result_type;

  /* The state must be initialized so that 0 < c < MWC_A1 - 1. */
  result_type x, c;
};

class MWC128 {
public:
  using state_type  = MWC128State;
  using result_type = state_type::result_type;

private:
  result_type const MWC_A1 {0xff3a275c007b8ee6};

public:
  MWC128() {}
  /*  Written in 2021 by Sebastiano Vigna (vigna@acm.org)

  To the extent possible under law, the author has dedicated all copyright
  and related and neighboring rights to this software to the public domain
  worldwide. This software is distributed without any warranty.

  See <http://creativecommons.org/publicdomain/zero/1.0/>. */

  /* This is a Marsaglia multiply-with-carry generator with period
     approximately 2^127. It is close in speed to a scrambled linear
     generator, as its only 128-bit operations are a multiplication and sum;
     is an excellent generator based on congruential arithmetic.

     As all MWC generators, it simulates a multiplicative LCG with
     prime modulus m = 0xff3a275c007b8ee5ffffffffffffffff and
     multiplier given by the inverse of 2^64 modulo m. The modulus has a
     particular form, which creates some theoretical issues, but at this
     size a generator of this kind passes all known statistical tests. For a
     generator of the same type with stronger theoretical guarantees
     consider a Goresky-Klapper generalized multiply-with-carry generator.
  */
  constexpr result_type generate() {
    const __uint128_t t = MWC_A1 * (__uint128_t)state.x + state.c;
    state.c = t >> 64;
    return state.x = t;
  }

private:
  state_type state;
};

struct MWC256State: public MWC128State {
  /* The state must be initialized so that 0 < c < MWC_A3 - 1. */
  result_type y, z;
};

class MWC256 {
public:
  using state_type  = MWC256State;
  using result_type = state_type::result_type;

private:
  result_type const MWC_A3 {0xff377e26f82da74a};

public:
  MWC256() {}
  /* This is a Marsaglia multiply-with-carry generator with period
     approximately 2^255. It is close in speed to a scrambled linear
     generator, as its only 128-bit operations are a multiplication and sum;
     is an excellent generator based on congruential arithmetic.

     As all MWC generators, it simulates a multiplicative LCG with prime
     modulus m = 0xff377e26f82da749ffffffffffffffffffffffffffffffffffffffffffffffff
     and multiplier given by the inverse of 2^64 modulo m. The modulus has a
     particular form, which creates some theoretical issues, but at this
     size a generator of this kind passes all known statistical tests. For a
     generator of the same type with stronger theoretical guarantees
     consider a Goresky-Klapper generalized multiply-with-carry generator.
  */

  result_type inline next() {
    const __uint128_t t = MWC_A3 * (__uint128_t)state.x + state.c;
    state.x = state.y;
    state.y = state.z;
    state.c = t >> 64;
    return state.z = t;
  }

private:
  state_type state;
};

#endif // MWC_HPP
