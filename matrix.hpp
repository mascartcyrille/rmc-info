#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

class Matrix: public std::vector<int> {
  public:
    Matrix(int size);
    void init();

private:
    int const size;
};

#endif // MATRIX_HPP
