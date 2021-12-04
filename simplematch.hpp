#ifndef SIMPLEMATCH_HPP
#define SIMPLEMATCH_HPP
#include <deque>
#include <cstdint>
#include <array>
#include "matrix.hpp"
#include "rng.hpp"

class SimpleMatch
{
private:
  std::deque<double> currentRandomSequence;
  std::deque<RNGState> states;
  RNG prng;
  int nbMatch;

public:

  // Constructor
  SimpleMatch(int size)
  {
    nbMatch = 0;
    for (int i = 0; i < size; i++)
    {
      states.push_back(prng.getState());
      currentRandomSequence.push_back(prng.generate());
    }
  }

  int getNbMatch()
  {
    return nbMatch;
  }

  /* Does the given matrix line (indexes from start to end) match the currentRandomSequence ?
 (exact match) */
  bool match(Matrix &mat, double treshold, int start, int end)
  {
    int i = 0;
    int j = start;
    nbMatch++; 
    bool match = true;
    while (j <= end && match)
    {
      int binValue = 1;
      if (currentRandomSequence[i] > treshold)
      {
        binValue = 0;
      }

      if (mat[j] != binValue)
      {
        match = false;
      }
      i++;
      j++;
    }
    return match;
  }

  // Shift both deques to the right
  void shift()
  {
    states.pop_front();
    currentRandomSequence.pop_front();
    states.push_back(prng.getState());
    currentRandomSequence.push_back(prng.generate());
  }

  // Return first state of deque states
  RNGState returnMatchingState()
  {
    return states.front();
  }
};

#endif // SIMPLEMATCH_HPP
