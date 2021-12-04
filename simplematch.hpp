#ifndef SIMPLEMATCH_HPP
#define SIMPLEMATCH_HPP
#include <deque>
#include <cstdint>
#include <array>
#include "matrix.hpp"
#include "rng.hpp"

template<class Gen>
class SimpleMatch
{
private:
  std::deque<typename Gen::result_type> currentRandomSequence;
  std::deque<typename Gen::state_type> states;
  RNG<Gen> prng;
  int nbMatch;

public:
  // Constructor
  SimpleMatch(int size): prng(), nbMatch(0) {
    for (int i = 0; i < size; i++)
    {
      states.push_back(prng.getState());
      currentRandomSequence.push_back(prng.generate());
    }
  }

public:
  int getNbMatch() {
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
  typename Gen::state_type returnMatchingState()
  {
    return states.front();
  }

  // Return average threshold
  double computeAverageThreshold(Matrix &mat, int size, int start, int end)
  {
    double sum = 0;
    for (int i = start; i <= end; i++)
    {
      sum = sum + mat[i];
    }
    return sum/size ;
  }

};

#endif // SIMPLEMATCH_HPP
