#include <array>
#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

int const lines = 10;
int const columns = 10;
std::vector<int> connection_matrix(lines * columns, 0);

double const proba = 0.5;

std::string output_file_name {"output_matrix.txt"};

int main() {
  std::cout << "This is the Random Matrix Compression INterference FunctiOn project!" << std::endl;

  { // Generate matrix
    std::mt19937_64 prng;
    std::uniform_real_distribution<double> unif(0., 1.);

    std::generate(connection_matrix.begin(), connection_matrix.end(), [&](){return (unif(prng)<=proba)? 1: 0;});

    for(int i {}; i < lines; ++i) {
      for(int j {}; j < lines; ++j) {
        std::cout << connection_matrix[i*lines + j] << " ";
      }
    }
  }

  { // Write matrix
    std::ofstream ofs(output_file_name);
    std::copy(connection_matrix.begin(), connection_matrix.end(), std::ostream_iterator<int>(ofs, " "));
    ofs.close();
  }

  return 0;
}
