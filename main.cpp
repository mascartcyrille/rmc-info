#include <array>
#include <random>
#include <iostream>
#include <algorithm>

int const lines = 10;
int const columns = 10;
std::array<int, lines*columns> connection_matrix {};

double const proba = 0.5;

int main() {
  std::cout << "This is the Random Matrix Compression INterference FunctiOn project!" << std::endl;

  std::mt19937_64 prng;
  std::uniform_real_distribution<double> unif(0., 1.);

  std::generate(connection_matrix.begin(), connection_matrix.end(), [&](){return (unif(prng)<=proba)? 1: 0;});

  for(int i {}; i < lines; ++i) {
    for(int j {}; j < lines; ++j) {
      std::cout << connection_matrix[i*lines + j] << " ";
    }
  }

  return 0;
}
