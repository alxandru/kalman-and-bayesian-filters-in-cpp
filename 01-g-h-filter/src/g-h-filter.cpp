#include <vector>
#include <iostream>
#include <fstream>
#include <random>

/*
  Performs g-h filter on 1 state variable with a fixed g and h.

  INPUT
    'data' contains vector with the measurements data to be filtered.
    'x0' is the initial value for our state variable
    'dx' is the initial change rate for our state variable
    'g' is the g-h's g scale factor
    'h' is the g-h's h scale factor
    'dt' is the length of the time step 
  OUTPUT
    'results' vector with the filter results
  
*/

std::vector<double> g_h_filter(const std::vector<double> &data, const double x0,
                              double dx, const double g,
                              const double h, const double dt = 1.) {
  std::vector<double> results;
  double xEst = x0;
  double xPred = 0.;
  double residual = 0.;
  for (const double &z : data) {
    // Prediction step
    xPred = xEst + dx * dt;

    // Update step
    residual = z - xPred;
    dx += h * (residual/dt);
    xEst = xPred + g * residual;
    results.push_back(xEst);
  }

  return results;
}

std::vector<double> gen_data(const double x0, const double dx,
                             const int count, const double noiseFactor) {
  std::vector<double> measurements;
  std::default_random_engine generator; 
  std::normal_distribution<double> distribution(0., 1.);
  double rnd_number = 0.;
  for(int i = 0; i < count; ++i) {
    rnd_number = x0 + dx*i + distribution(generator) * noiseFactor;
    measurements.push_back(rnd_number);
  }
  return measurements;
}


std::vector<double> gen_data_with_acceleration(const double x0, const double dx,
                             const int count, const double noiseFactor, const double accel) {
  std::vector<double> measurements;
  std::default_random_engine generator; 
  std::normal_distribution<double> distribution(0., 1.);
  double rnd_number = 0.;
  for(int i = 0; i < count; ++i) {
    rnd_number = x0 + accel * pow(i, 2) / 2 + dx*i + distribution(generator) * noiseFactor;
    measurements.push_back(rnd_number);
  }
  return measurements;
}

template<typename T>
std::vector<double> linspace(const T start, const T end, const int points) {
  double space = (end - start) / (points - 1);
  std::vector<double> linspaced;
  for (int i = 0; i < points -1; ++i){
    linspaced.push_back(start + space * i);
  }
  linspaced.push_back(end);
  return linspaced;
}

// Trackin a Train methods

double compute_new_position(const double position, const double velocity, const double dt = 1.) {
  return position + velocity * dt;
}

double measure_position(const double position, const double rand) {
  return position + rand * 500;
}

std::vector<double> gen_train_data(const double position, const double velocity, const int count) {
  std::vector<double> zs;
  std::default_random_engine generator; 
  std::normal_distribution<double> distribution(0., 1.);

  for (int i = 0; i < count; ++i) {
    double rand = distribution(generator);
    double pos = compute_new_position(position, velocity);
    zs.push_back(measure_position(pos, rand));
  }
  return zs;
}

std::vector<double> gen_train_data_with_acc(const double position, const double velocity, const int count) {
  std::vector<double> zs;
  std::default_random_engine generator; 
  std::normal_distribution<double> distribution(0., 1.);
  double vel = velocity;

  for (int i = 0; i < count; ++i) {
    double rand = distribution(generator);
    double pos = compute_new_position(position, vel);
    vel += 0.2;
    zs.push_back(measure_position(pos, rand));
  }
  return zs;
}

int main() {

  std::vector<double> measurements{158.0, 164.2, 160.3, 159.9, 162.1, 164.6, 169.6, 167.4, 166.4, 171.0, 171.2, 172.6};
  std::vector<double> results = g_h_filter(measurements, 160., 1., 6./10, 2./3);

  std::ofstream ofs("../data/g-h-filter1.dat");
  ofs << "# data" << std::endl;
  double hiddenMeasurement = 160.;
  for (int i = 0; i < 12; ++i) {
    ofs << hiddenMeasurement++ << " " << measurements[i] << " " << results[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  // Create measurement function
  results.clear(); measurements.clear();
  int N = 30;
  measurements = gen_data(0, 1, N, 1);
  results = g_h_filter(measurements, 0., 1., .2, .02);
  ofs.open("../data/g-h-filter2.dat");
  ofs << "# data" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << measurements[i] << " " << results[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  // Bad Initial Conditions
  results.clear(); measurements.clear();
  N = 100;
  measurements = gen_data(5., 2., N, 10);
  results = g_h_filter(measurements, 100., 2., .2, .02);
  ofs.open("../data/g-h-filter3.dat");
  ofs << "# data" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << measurements[i] << " " << results[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  //  Extreme Noise
  results.clear(); measurements.clear();
  N = 100;
  measurements = gen_data(5., 2., N, 100);
  results = g_h_filter(measurements, 5., 2., .2, .02);
  ofs.open("../data/g-h-filter4.dat");
  ofs << "# data" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << measurements[i] << " " << results[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  // The Effect of Acceleration
  results.clear(); measurements.clear();
  N = 20;
  measurements = gen_data_with_acceleration(10., 0., N, 0, 9.);
  results = g_h_filter(measurements, 10., 0., .2, .02);
  ofs.open("../data/g-h-filter5.dat");
  ofs << "# data" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << measurements[i] << " " << results[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  // Varying g
  measurements.clear();
  N = 50;
  measurements = gen_data(5., 5., N, 50);
  std::vector<double> results1 = g_h_filter(measurements, 0., 5., .1, .01);
  std::vector<double> results2 = g_h_filter(measurements, 0., 5., .4, .01);
  std::vector<double> results3 = g_h_filter(measurements, 0., 5., .8, .01);
  ofs.open("../data/g-h-filter6.dat");
  ofs << "# data" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << measurements[i] << " " << results1[i] << " " << results2[i] << " " << results3[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  // Measurement signal changes not due to noise, but an actual state change
  measurements.clear();
  double measurement = 5;
  for (int i = 0; i < 10; ++i) {
    measurements.push_back(measurement++);
  }
  for (int i = 0; i < 40; ++i) {
      measurements.push_back(measurement);
  }
  results1.clear(); results2.clear(); results3.clear(); 
  results1 = g_h_filter(measurements, 0., 4., .1, .01);
  results2 = g_h_filter(measurements, 0., 4., .5, .01);
  results3 = g_h_filter(measurements, 0., 4., .9, .01);
  ofs.open("../data/g-h-filter7.dat");
  ofs << "# data" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << measurements[i] << " " << results1[i] << " " << results2[i] << " " << results3[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  // Benedict-Bordner filter 
  measurements.clear();
  measurements = {5,6,7,8,9,9,9,9,9,10,11,12,13,14,15,16,16,16,16,16,16,16,16,16,16,16};
  N = 26;
  results1.clear(); results2.clear(); 
  results1 = g_h_filter(measurements, 4., 1., .302, .054);
  results2 = g_h_filter(measurements, 4., 1., .546, .205);
  ofs.open("../data/g-h-filter8.dat");
  ofs << "# data" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << measurements[i] << " " << results1[i] << " " << results2[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  // Varying h
  measurements.clear();
  N = 50;
  measurements = linspace(0., 1., N);
  results1.clear(); results2.clear(); results2.clear();
  results1 = g_h_filter(measurements, 0., 0., .2, .05);
  results2 = g_h_filter(measurements, 0., 2., .2, .05);
  results3 = g_h_filter(measurements, 0., 2., .2, .5);
  ofs.open("../data/g-h-filter9.dat");
  ofs << "# data" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << measurements[i] << " " << results1[i] << " " << results2[i] << " " << results3[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  ofs.close();

  // Example: Tracking a Train
  double pos = 23. * 1000;
  double vel = 15.;
  N = 100;
  std::vector<double> zs = gen_train_data(pos, vel, N);
  results.clear();
  results = g_h_filter(zs, pos, 15., .01, .0001);
  ofs.open("../data/g-h-tracking-train.dat");
  ofs << "# data1" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << zs[i] << " " << results[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  
  results.clear();
  results = g_h_filter(zs, pos, 15., .02, .0001);
  ofs << "# data2" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << zs[i] << " " << results[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;
  
  results.clear();
  results = g_h_filter(zs, pos, 15., .01, .1);
  ofs << "# data3" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << zs[i] << " " << results[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;

  zs.clear(); results.clear();
  zs = gen_train_data_with_acc(pos, vel, N);
  results = g_h_filter(zs, pos, 15., .01, .0001);
  ofs << "# data4" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << zs[i] << " " << results[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;

  results.clear();
  results = g_h_filter(zs, pos, 15., .01, .001);
  ofs << "# data5" << std::endl;
  for (int i = 0; i < N; ++i) {
    ofs << " " << zs[i] << " " << results[i] << std::endl;
  }
  ofs << std::endl << std::endl << std::endl;

  ofs.close();

  return 0;
}
