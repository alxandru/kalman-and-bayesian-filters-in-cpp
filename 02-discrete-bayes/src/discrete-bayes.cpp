#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>

const std::vector<double> normalize(const std::vector<double> &posterior) {
  std::vector<double> norm(posterior.size());
  double sum = std::accumulate(posterior.begin(), posterior.end(), 0.);
  for (int i = 0; i < posterior.size(); ++i){
    norm[i] = posterior[i] / sum;
  }
  return norm;
}

const std::vector<double> update(const std::vector<double> &likelihood,
                                 const std::vector<double> &belief) {
  std::vector<double> posterior(likelihood.size());
  for(int i = 0; i < likelihood.size(); ++i) {
    posterior[i] = likelihood[i] * belief[i];
  }
  return normalize(posterior);
}

/*
Compute likelihood that a measurement matches
positions in the hallway.
*/
std::vector<double> likelihood_hallway(const std::vector<int> &hall,
                                       const int z, const double z_prob) {
  std::vector<double> likelihood(hall.size(), 1.);
  double scale = z_prob / (1. - z_prob);
  for(int i = 0; i < hall.size(); ++i) {
    if (hall[i] == z) {
      likelihood[i] *= scale;
    }
  }
  return likelihood;
}

int main() {
  std::vector<int> hallway{1, 1, 0, 0, 0, 0, 0, 0, 1, 0};
  std::vector<double> belief(10, 0.1);

  std::vector<double> likelihood = likelihood_hallway(hallway, 1, .75);
  std::vector<double> posterior = update(likelihood, belief);

  for (const double &el: posterior){
    std::cout << el << std::endl;
  }

  std::ofstream ofs("../data/discrete-bayes1.dat");
  ofs << "# data1" << std::endl;
  for (const double &el: posterior){
    ofs << el << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  return 0;
}