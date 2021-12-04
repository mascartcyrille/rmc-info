#ifndef RNG_HPP
#define RNG_HPP

#include <array>
#include <random>
#include <iostream>
#include <iterator>

struct RNGState {
  typedef unsigned long long result_type;

  static int const NN = 312;

  RNGState(): mti(NN+1) {}

  /**
   * @brief The array for the state vector
   */
  result_type mt[NN];
  /**
   * @brief mti==NN+1 means mt[NN] is not initialized
   */
  int mti;
};

class RNG {
public:
  typedef unsigned long long result_type;

public:
  /**
   * @brief initializes mt[NN] with a seed
   * @param seed
   */
  RNG(result_type sd = default_seed): state() {
    state.mt[0] = sd;
    for (state.mti=1; state.mti<NN; state.mti++)
      state.mt[state.mti] =  (6364136223846793005ULL * (state.mt[state.mti-1] ^ (state.mt[state.mti-1] >> 62)) + state.mti);
  }

  RNG(RNGState const& rngst): state(rngst) {}

  /**
   * @brief initialize by an array with array-length
   * @param init_key is the array for initializing keys
   * @param key_length is its length
   */
  RNG(result_type init_key[], result_type key_length)
    : RNG(default_seed) {
    result_type i {1};
    result_type j {0};

    for (result_type k = NN>key_length ? NN : key_length; k; --k) {
      state.mt[i] = (state.mt[i] ^ ((state.mt[i-1] ^ (state.mt[i-1] >> 62)) * 3935559000370003845ULL))
          + init_key[j] + j; /* non linear */
      i++; j++;
      if (i>=NN) { state.mt[0] = state.mt[NN-1]; i=1; }
      if (j>=key_length) j=0;
    }
    for (result_type k=NN-1; k; --k) {
      state.mt[i] = (state.mt[i] ^ ((state.mt[i-1] ^ (state.mt[i-1] >> 62)) * 2862933555777941757ULL))
          - i; /* non linear */
      i++;
      if (i>=NN) { state.mt[0] = state.mt[NN-1]; i=1; }
    }

    state.mt[0] = 1ULL << 63; /* MSB is 1; assuring non-zero initial array */
  }

  ~RNG() = default;

  RNGState getState() {
    return state;
  }

  void setState(RNGState& st) {
    state = st;
  }

  /**
   * @brief generates a random number on [0, 2^64-1]-interval
   * @return
   */
  double generate() {
    result_type x;

    if (state.mti >= NN) { /* generate NN words at one time */
      for (int i=0;i<NN-MM;i++) {
        x = (state.mt[i]&UM)|(state.mt[i+1]&LM);
        state.mt[i] = state.mt[i+MM] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
      }
      for (int i = NN-MM;i<NN-1;i++) {
        x = (state.mt[i]&UM)|(state.mt[i+1]&LM);
        state.mt[i] = state.mt[i+(MM-NN)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
      }
      x = (state.mt[NN-1]&UM)|(state.mt[0]&LM);
      state.mt[NN-1] = state.mt[MM-1] ^ (x>>1) ^ mag01[(int)(x&1ULL)];

      state.mti = 0;
    }

    x = state.mt[state.mti++];

    x ^= (x >> 29) & 0x5555555555555555ULL;
    x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
    x ^= (x << 37) & 0xFFF7EEE000000000ULL;
    x ^= (x >> 43);

    return (x >> 11) * (1.0/9007199254740991.0);
  }

private:
  static int const NN = 312;
  static int const MM = 156;
  static result_type const MATRIX_A = 0xB5026F5AA96619E9ULL;
  /**
   * Most significant 33 bits
   */
  static result_type const UM = 0xFFFFFFFF80000000ULL;
  /**
   * Least significant 31 bits
   */
  static result_type const LM = 0x7FFFFFFFULL;

  static result_type const default_seed = 19650218ULL;

  result_type const mag01[2]={0ULL, MATRIX_A};

private:
  RNGState state;
};



#endif // RNG_HPP
