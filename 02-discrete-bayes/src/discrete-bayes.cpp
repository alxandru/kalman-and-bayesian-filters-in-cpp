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

std::vector<double> predict_move(const std::vector<double> &belief,
                                const int move, const double p_under,
                                const double p_correct, const double p_over) {
  int n = belief.size();
  std::vector<double> prior(n, 0.);
  for(int i = 0; i < n; ++i) {
    prior[i] = belief[(i - move) % n] * p_correct +
              belief[(i - move - 1) % n] * p_over +
              belief[(i - move + 1) % n] * p_under;
  }
  return prior;
}

std::vector<double> predict_move_convolution(const std::vector<double> &belief,
                                const std::vector<double> &kernel,
                                const int offset) {
  int N = belief.size();
  int kN = kernel.size();
  int width = (kN - 1) / 2;

  std::vector<double> prior(N, 0.);
  int idx = 0;
  for(int i = 0; i < N; ++i) {
    for(int k = 0; k < kN; ++k) {
      idx = (i + (width - k) - offset) % N;
      if (idx < 0) {
        idx = N + idx;
      }
      //std::cout << "i: " << i << " idx: " << idx << " k: " << k << std::endl;
      prior[i] += belief[idx] * kernel[k];
    }
  }
  return prior;
}

int main() {
  std::vector<int> hallway{1, 1, 0, 0, 0, 0, 0, 0, 1, 0};
  std::vector<double> belief(10, 0.1);

  std::vector<double> likelihood = likelihood_hallway(hallway, 1, .75);
  std::vector<double> posterior = update(likelihood, belief);

  // for (const double &el: posterior){
  //   std::cout << el << std::endl;
  // }

  std::ofstream ofs("../data/discrete-bayes1.dat");
  ofs << "# data" << std::endl;
  int idx = 0;
  for (const double &el: posterior){
    ofs << idx++ << " " << el << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  belief = {0., 0., .4, .6, 0., 0., 0., 0., 0., 0.};
  ofs.open("../data/discrete-bayes2.dat");
  ofs << "# belief" << std::endl;
  idx = 0;
  for (const double &el: belief){
    ofs << idx++ << " " << el << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  std::vector<double> prior = predict_move(belief, 2, .1, .8, .1);
  ofs << "# move" << std::endl;
  idx = 0;
  for (const double &el: prior){
    ofs << idx++ << " " << el << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  belief = {.05, .05, .05, .05, .55, .05, .05, .05, .05, .05};
  std::vector<double> kernel = {.1, .8, .1};
  ofs.open("../data/discrete-bayes3.dat");
  ofs << "# belief" << std::endl;
  idx = 0;
  for (const double &el: belief){
    ofs << idx++ << " " << el << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  std::vector<double> prior1 = predict_move_convolution(belief, kernel, 1);
  ofs << "# move" << std::endl;
  idx = 0;
  for (const double &el: prior1){
    std::cout << idx++ << " " << el << std::endl;
    ofs << idx++ << " " << el << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  return 0;
}