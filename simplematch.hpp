#ifndef SIMPLEMATCH_HPP
#define SIMPLEMATCH_HPP
#include <deque>
#include <cstdint>


class simpleMatch {
  private: 
    std::deque<double> currentRandomSequence;
    std::deque<std::uint_fast64_t[]> states;
  public:
    simpleMatch(); // Constructor
    void shift();
};

#endif // SIMPLEMATCH_HPP
