#include <iostream>
#include <fstream>
#include <random>

using namespace std;

vector<int> neurons    = { 0 };
const double STARTTIME = 0.0;
const double ENDTIME   = 100.0;
const double STEPSIZE  = 0.05;

double rand(double min, double max) {
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<> dist(min, max+1);
  return dist(gen);
}

double rand(double max) {
  return rand(0, max);
}

int main(int argc, char *argv[])
{
  if(argc < 2) { exit(0); }
  ofstream f(argv[1]);
  
  f << "time";
  for(int i : neurons) { f << ',' << i; }
  f << '\n';
  for(double t = STARTTIME; t <= ENDTIME; t+=STEPSIZE) {
    f << t;
    for(vector<int>::size_type iter = 0; iter < neurons.size(); ++iter) {
      f << ',' << rand(0, 30);
    }
    f << '\n';
  }
  
  f.close();
}
