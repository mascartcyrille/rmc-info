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

/*Le plan
-Optimal threshold
-threshold seg
-error margin (compression avec pertes)
-error margin (compression + dictionnaire)
-parallelisation
*/

double const threshold = 0.5;

std::string const output_file_name{"output_matrix.txt"};

int main(int argc, char **argv)
{
  std::cout << "This is the Random Matrix Compression INterference FunctiOn project!" << std::endl;

  int const size = 100;

  Matrix connection_matrix(size);

  { // Load Matrix
    if (argc >= 2)
    {
      MatrixLoader ml;
      ml.readFile(&connection_matrix, argv[1]);
    }
    else
    { // Generate matrix
      RNG prng(5);
      connection_matrix.initRandom(threshold, prng);
    }
  }
  /*
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
*/
  { // Display matrix to console
    for (auto el : connection_matrix)
    {
      std::cout << el << " ";
    }
    std::cout << "\n";
  }

  { // Write matrix
    std::ofstream ofs(output_file_name);
    std::copy(connection_matrix.begin(), connection_matrix.end(), std::ostream_iterator<int>(ofs, " "));
    ofs.close();
  }

  //Match
  SimpleMatch sm(size);
  bool full = false;
  CompressedMatrix cm(size);

  int loop = 0, limit = 10000000;
  do
  {
    int start = 0, end = size - 1;
    for (int i = 0; i < size; i++)
    {
      if (cm[i].second == false)
      {
        // bool match = sm.match(connection_matrix, sm.computeAverageThreshold(connection_matrix, size, start, end), start, end) ;
        bool match = sm.match(connection_matrix, 0.5, start, end);
        if (match)
        {
          cm.setElement(i, sm.returnMatchingState());
          std::cout << "Found match for line " << i << " in loop " << loop << "\n";
        }
      }
      start = start + size;
      end = end + size;
    }
    full = cm.full();
    sm.shift();
    loop++;
  } while (!full && loop < limit);

  //Reconstruct matrix
  Matrix reconstructed_matrix(size);
  reconstructed_matrix.initFromCompressed(cm, 0.5);

  { // Display matrix to console
    for (auto el : reconstructed_matrix)
    {
      std::cout << el << " ";
    }
    std::cout << "\n";
  }

  return 0;
}
