#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <random>

using namespace std;

#define NEURON_SIZE 100
#define SYNAPSE_SIZE 1000

string neuron_file = "nsets.conf";
string synapse_file = "ssets.conf";

// Sample NSETS.CONF 
// v:-30,m:0,n:0,h:0,gna:120,ena:115,gk:36,ek:-12,gl:0.3,el:10.6,iext:10
// v:-30,m:0,n:0,h:0,gna:120,ena:115,gk:36,ek:-12,gl:0.3,el:10.6,iext:10

// Sample SSETS.CONF
// g1:0,g2:0,gsyn:0.1,tau1:10,tau2:100,esyn:20,pre:1,post:2
// g1:0,g2:0,gsyn:0.1,tau1:10,tau2:100,esyn:20,pre:2,post:1

vector<string> nvars     = {"v", "m", "n", "h", "gna", "ena", "gk", "ek", "gl", "el", "iext"};
vector<double> nvals_min = {-20, 0,   0,   0,   120,   115,   36,   -12,  0.3,  10.6};
vector<double> nvals_max = {-70, 1,   1,   1,   120,   115,   36,   -12,  0.3,  10.6};

vector<string> svars     = {"g1", "g2", "gsyn", "tau1", "tau2", "esyn", "pre", "post"};
vector<double> svals_min = {0,    0,    0.1,    10,     100,    -40};
vector<double> svals_max = {0,    0,    10.1,   10,     100,    40};

double get_rand(double min, double max) {
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dist(min, max+1);
  
  return dist(gen);
}

vector<int> gen_neuron_size_rand_numbers() {
  vector<int> r(NEURON_SIZE, 1);
  
  for(int i = 0; i < SYNAPSE_SIZE - NEURON_SIZE; ++i) {
      r[get_rand(0, NEURON_SIZE-1)]++;
  }

  return r;
}

void generate_random_neurons() {
  fstream neuron_stream;
  neuron_stream.open(neuron_file, ios::out);
  for(long i = 0; i < NEURON_SIZE; ++i) {
    for(vector<string>::size_type j = 0; j < nvars.size(); ++j) {      
      neuron_stream << nvars.at(j) << ":"
                    << ((j < nvars.size()-1) ? get_rand(nvals_min.at(j), nvals_max.at(j)) : ((i!=0)?0:10));
    }
    neuron_stream << endl;
  }
  neuron_stream.close();
}

void generate_random_synapse() {
  fstream synapse_stream;
  synapse_stream.open(synapse_file, ios::out);
  vector<int> gen_neuron_size_rand_numbers();
  for(long i = 0; i < SYNAPSE_SIZE; ++i) {
    for(vector<string>::size_type j = 0; j < svars.size(); ++j) {
      synapse_stream << svars.at(j) << ":"
                    << ((j < svars.size()-2) ? get_rand(nvals_min.at(j), nvals_max.at(j)) : 10);
    }
    synapse_stream << endl;
  }
  synapse_stream.close();
}

int main(int argc, char *argv[]) {
  // generate_random_neurons();
  // generate_random_synapse();
  int sum = 0;
  for(int val : gen_neuron_size_rand_numbers()) {
    cout << val << "\n";
    sum+=val;
  }
  
  cout<<endl<<sum<<endl;
  return 0;
}
