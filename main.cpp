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

#include "mwc.hpp"
#include "rng.hpp"
#include "mt19937.hpp"
#include "xoshiro.hpp"

/*Le plan
-Optimal threshold
-threshold seg
-error margin (compression avec pertes)
-error margin (compression + dictionnaire)
-essayer de compresser le Yale format ?
-essayer des générateurs + pourris
-parallelisation
*/

double const genThreshold = 0.5;

std::string const output_file_name{"output_matrix.txt"};

int main(int argc, char **argv)
{
  std::cout << "This is the Random Matrix Compression INterference FunctiOn project!" << std::endl;

  int const size = 10;

  Matrix connection_matrix(size);

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

  int loop = 0, limit = 10000;
  do
  {
    int start = 0, end = size - 1;
    for (int i = 0; i < size; i++)
    {
      if (std::get<1>(cm[i]) == false)
      {
        // Single fixed treshold
        double fixedMatchTreshold = 0.5 ;
        bool match = sm.match(connection_matrix, fixedMatchTreshold, start, end) ;
        if (match)
        {
          cm.setElement(i, sm.returnMatchingState(), fixedMatchTreshold);
          std::cout << "Found match for line " << i << " in loop " << loop << " and treshold " << fixedMatchTreshold << "\n";
        }

        // Single avg treshold => jolie amélioration par rapport au fixed en nombre de tours de boucle pour trouver
        /*double avgTreshold = sm.computeAverageThreshold(connection_matrix, size, start, end) ;
        //std::cout<<avgTreshold<<"\n";
        bool match = sm.match(connection_matrix, avgTreshold, start, end) ;
        if (match)
        {
          cm.setElement(i, sm.returnMatchingState(), avgTreshold);
          std::cout << "Found match for line " << i << " in loop " << loop << " and treshold " << avgTreshold << "\n";
        }*/

        // Multiple fixed tresholds => trouvé en encore nettement moins de boucles, mais comme on fait aussi beaucoup plus de matching, faut voir la rentabilité.
        // Faudrait aussi voir à quel point on est loin de l'avg treshold à chaque fois
        /*double curTresh = 0.2;
        bool match = false;
        while (curTresh < 1 && !match)
        {
          match = sm.match(connection_matrix, curTresh, start, end);
          if (match)
          {
            cm.setElement(i, sm.returnMatchingState(), curTresh);
            std::cout << "Found match for line " << i << " in loop " << loop << " and treshold " << curTresh << "\n";
          }
          curTresh = curTresh + 0.2;
        }*/

        //Multiple around avg tresholds => pas d'amélioration visible par rapport à multiple fixed tresholds, voire même dégradation.
        /*double avgTreshold = sm.computeAverageThreshold(connection_matrix, size, start, end);
        bool match = sm.match(connection_matrix, avgTreshold, start, end);
        if (match)
        {
          cm.setElement(i, sm.returnMatchingState(), avgTreshold);
          std::cout << "Found match for line " << i << " in loop " << loop << " and treshold " << avgTreshold << "\n";
        }
        else
        {
          //We only explore around avg treshold
          double curTreshMax = avgTreshold + 0.2;
          double curTreshMin = avgTreshold - 0.2;
          while (curTreshMax < 1 && curTreshMin > 0 && !match)
          {
            match = sm.match(connection_matrix, curTreshMax, start, end);
            if (match)
            {
              cm.setElement(i, sm.returnMatchingState(), curTreshMax);
              std::cout << "Found match for line " << i << " in loop " << loop << " and treshold " << curTreshMax << "\n";
            }
            else
            {
              match = sm.match(connection_matrix, curTreshMin, start, end);
              if (match)
              {
                cm.setElement(i, sm.returnMatchingState(), curTreshMin);
                std::cout << "Found match for line " << i << " in loop " << loop << " and treshold " << curTreshMin << "\n";
              }
            }
            curTreshMax = curTreshMax + 0.2;
            curTreshMin = curTreshMin - 0.2;
          }
        }*/
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
  }

  return 0;
}
