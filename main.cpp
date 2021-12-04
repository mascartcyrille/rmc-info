#include <array>
#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "rng.hpp"
#include "matrix.hpp"
#include "simplematch.hpp"
#include "matrixloader.hpp"
#include "compressedmatrix.hpp"

double const threshold = 0.5;

std::string const output_file_name {"output_matrix.txt"};

int main(int argc, char** argv) {
  std::cout << "This is the Random Matrix Compression INterference FunctiOn project!" << std::endl;

  Matrix connection_matrix(10);

  { // Load Matrix
    if(argc >= 2) {
      MatrixLoader ml;
      ml.readFile(&connection_matrix, argv[1]);
    } else { // Generate matrix
      RNG prng;
      connection_matrix.initRandom(threshold, prng);
    }
  }

  { // Test PRNG class
    RNG prng;
    RNGState state = prng.getState();
    for(int i = 0; i < 10; ++i) {
      std::cout << prng.generate() << " ";
    }
    std::cout << "\n";

    prng.setState(state);
    for(int i = 0; i < 10; ++i) {
      std::cout << prng.generate() << " ";
    }
    std::cout << "\n";
  }

  { // Display matrix to console
    for(auto el: connection_matrix) {
        std::cout << el << " ";
    }
    std::cout << "\n";
  }

  { // Write matrix
    std::ofstream ofs(output_file_name);
    std::copy(connection_matrix.begin(), connection_matrix.end(), std::ostream_iterator<int>(ofs, " "));
    ofs.close();
  }

  return 0;
}
