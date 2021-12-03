#include <array>
#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "matrix.hpp"
#include "simplematch.hpp"
#include "matrixloader.hpp"
#include "compressedmatrix.hpp"

double const treshold = 0.5;

std::string const output_file_name {"output_matrix.txt"};

int main(int argc, char** argv) {
  std::cout << "This is the Random Matrix Compression INterference FunctiOn project!" << std::endl;

  Matrix connection_matrix(10);

  { // Load Matrix
    if(argc >= 2) {
      std::ifstream ifs(argv[1]);
      connection_matrix.clear();
      std::copy(std::istream_iterator<int>(ifs), std::istream_iterator<int>(), std::back_inserter(connection_matrix));
      ifs.close();
    } else { // Generate matrix
      std::mt19937_64 prng;
      std::uniform_real_distribution<double> unif(0., 1.);

      std::generate(connection_matrix.begin(), connection_matrix.end(), [&](){return (unif(prng)<=treshold)? 1: 0;});
    }
  }

  { // Display matrix to console
    for(auto el: connection_matrix) {
        std::cout << el << " ";
    }
  }

  { // Write matrix
    std::ofstream ofs(output_file_name);
    std::copy(connection_matrix.begin(), connection_matrix.end(), std::ostream_iterator<int>(ofs, " "));
    ofs.close();
  }

  return 0;
}
