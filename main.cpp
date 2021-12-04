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
#include "matchlines.hpp"

/*Le plan
-Optimal threshold
-threshold seg
-error margin (compression avec pertes)
-error margin (compression + dictionnaire)
-essayer de compresser le Yale format ?
-essayer des générateurs + pourris
-parallelisation
-Si on trouve des jolis matchs pour des matrices grandes => on peut les sauvegarder pour simplifier des traitements futurs ?
-Couper la matrice en plus petits morceaux => rentabilité en fonction de la taille des états du générateur
*/

double const genThreshold = 0.2;

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
      connection_matrix.initRandom(genThreshold, prng);
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

    std::cout << "Tresholds : ";
    for (int i = 0; i < size; i++)
    {
      std::cout << connection_matrix.getAverageThreshold(i) << " ";
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
  MatchLines ml;
  bool full = false;
  CompressedMatrix cm(size);

  int loop = 0, limit = 100000;
  do
  {
    int start = 0, end = size - 1;
    for (int i = 0; i < size; i++)
    {
      if (std::get<1>(cm[i]) == false)
      {
        //ml.fixedTreshMatch(connection_matrix, cm, i, start, end, sm, 0.2, loop);
        //ml.avgTreshMatch(connection_matrix, cm, i, start, end, sm, loop);
        ml.avgTreshMatchApprox(connection_matrix, cm, i, start, end, sm, 10, loop);
        //ml.stepTreshMatch(connection_matrix, cm, i, start, end, sm, 0.2, loop); //Faire des stepMatch C PA BIEN
        //ml.avgStepTreshMatch(connection_matrix, cm, i, start, end, sm, 0.2, 2, loop);
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
  reconstructed_matrix.initFromCompressed(cm);

  { // Display matrix to console
    for (auto el : reconstructed_matrix)
    {
      std::cout << el << " ";
    }
    std::cout << "\n";

    std::cout << "Tresholds : ";
    for (int i = 0; i < size; i++)
    {
      std::cout << reconstructed_matrix.getAverageThreshold(i) << " ";
    }
    std::cout << "\n";
  }

  return 0;
}
