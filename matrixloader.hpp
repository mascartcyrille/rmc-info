#ifndef MATRIXLOADER_HPP
#define MATRIXLOADER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "matrix.hpp"

class MatrixLoader
{
public:
  MatrixLoader() {}

public:
  void readFile(Matrix* matrix, std::string file_name) {
    std::ifstream ifs(file_name);

    matrix->clear();
    std::copy(std::istream_iterator<int>(ifs), std::istream_iterator<int>(), std::back_inserter(*matrix));

    ifs.close();
  }
};

#endif // MATRIXLOADER_HPP
