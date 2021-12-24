#include <array>
#include <tuple>
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
#include "matchlines.hpp"

// #include "mwc.hpp"
#include "rng.hpp"
#include "mt19937.hpp"
// #include "xoshiro.hpp"

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

/**
 * @brief genThreshold
 */
double const genThreshold = 0.2;

/**
 * @brief
 */
std::string const output_file_name{"output_matrix.txt"};

template<class Gen>
void print_state(typename Gen::state_type const& prng, std::string name = "") {
  std::cout << "\n########## Display PRNG " << name << " ##########\n";
  std::cout << "prng -> ";
  for(auto i: prng.mt) {
    std::cout << i << " ";
  }
  std::cout << prng.mti << "\n";
}

void print_matrix(Matrix const& mat, std::string name = "") {
  std::cout << "\n########## Matrix " << name << " ##########\n";
  for (auto el_it = mat.cbegin(); el_it != mat.cend(); ++el_it)
  {
    std::cout << *el_it << " ";
  }
  std::cout << "\n";
}

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{
  std::cout << "This is the Random Matrix Compression INterference FunctiOn project!" << std::endl;

  int const size = 100;

  Matrix connection_matrix(size);
  //  Matrix connection_matrix2(size);
  //  Matrix test_matrix(size);
  //  MT19937::state_type* state_test;
  //  MT19937::state_type rec_test;

  { // Load Matrix
    if (argc >= 2)
    {
      MatrixLoader ml;
      ml.readFile(&connection_matrix, argv[1]);
    }
    else
    { // Generate matrix
      RNG<MT19937> prng(5);

      connection_matrix.initRandom(genThreshold, prng);
    }

    print_matrix(connection_matrix, "original");
  }

  { // Write matrix
    std::ofstream ofs(output_file_name);
    std::copy(connection_matrix.begin(), connection_matrix.end(), std::ostream_iterator<int>(ofs, " "));
    ofs.close();
  }

  {//Match
    SimpleMatch<MT19937> sm(size);
    MatchLines ml;
    bool full = false;
    CompressedMatrix<MT19937> cm(size);

    int loop = 0, limit = 10000;
    do
    {
      int start = 0, end = size - 1;
      for (int i = 0; i < size; i++)
      {
        if (std::get<1>(cm[i]) == false)
        {
          //ml.fixedTreshMatch(connection_matrix, cm, i, start, end, sm, 0.2, loop);
          ml.avgTreshMatch(connection_matrix, cm, i, start, end, sm, loop);
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

    { //Reconstruct matrix
      Matrix reconstructed_matrix(size);
      reconstructed_matrix.initFromCompressed(cm);

      { // Display matrix to console
        print_matrix(reconstructed_matrix, "reconstructed");

        std::cout << "Tresholds : ";
        for (int i = 0; i < size; i++)
        {
          std::cout << reconstructed_matrix.getAverageThreshold(i) << " ";
        }
        std::cout << "\n";
      }
    }
  }

  return 0;
}
