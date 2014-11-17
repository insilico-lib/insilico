#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <assert.h>

#define NEURON 1
#define SYNAPSE 2

using namespace std;

vector<long> neuron_list_ids;
vector<double> synapse_list_ids;

vector<string> var_list_ids;
vector<double> var_vals;

vector<long> pre_neuron, post_neuron;

double get_value(long id, string variable, int mode) {
  long startindex, endindex;

  switch(mode) {
    case NEURON: {
      startindex = neuron_list_ids.size()>(id-1)?neuron_list_ids.at(id-1):0;
      endindex = neuron_list_ids.size()>(id)?neuron_list_ids.at(id):startindex+1;
      break;
    }
    case SYNAPSE: {
      startindex = synapse_list_ids.size()>(id-1)?synapse_list_ids.at(id-1):0;
      endindex = synapse_list_ids.size()>(id)?synapse_list_ids.at(id):startindex+1;
      break;
    }
  }
  for(long iter=startindex;iter<endindex;iter++) {
    if(variable.compare(var_list_ids[iter]) == 0) {
      return var_vals[iter];
    }
  }
  return 0.0;
}

int main(){
  ifstream stream1("nsets.conf"), stream2("ssets.conf");
  string str, c_var;
  cout<<"nsets\n";
  long ncount=0;
  
  while(getline(stream1,str) > 0) {
    cout<<ncount<<" reading> "<<str<<endl;
    neuron_list_ids.push_back(ncount);
    for (int i=0; i<str.length();++i) {
      c_var = "";
      while(i<str.length() && str.at(i)!=':') {
        c_var=c_var+str.at(i);
        i++;
      }
      var_list_ids.push_back(c_var);

      c_var = "";
      i++;
      while(i<str.length() && str.at(i)!=',') {
        c_var=c_var+str.at(i);
        i++;
      }
      size_t sz;
      var_vals.push_back(stod(c_var,&sz));

      cout<<var_list_ids.at(ncount)<<endl;
      cout<<var_vals.at(ncount)<<endl;

      ncount++;
    }
  }
  bool pre=false, post=false;
  cout<<"ssets\n";
  while(getline(stream2,str) > 0){
    cout<<"reading> "<<str<<endl;
    synapse_list_ids.push_back(ncount);
    for (int i=0; i<str.length();++i) {
      c_var = "";
      while(i<str.length() && str.at(i)!=':') {
        c_var=c_var+str.at(i);
        i++;
      }
      if(c_var.compare("pre")==0){
        pre=true; post=false;
      }
      else if(c_var.compare("post")==0){
        pre=false; post=true;
      }
      else {
        var_list_ids.push_back(c_var);
        pre=false; post=false;
      }
      c_var = "";
      i++;
      while(i<str.length() && str.at(i)!=',') {
        c_var=c_var+str.at(i);
        i++;
      }
      size_t sz;
      if(pre==true) {
        pre_neuron.push_back(stoi(c_var,&sz));
      }
      else if(post==true){
        post_neuron.push_back(stoi(c_var,&sz));
      }
      else {
        var_vals.push_back(stod(c_var,&sz));
        cout<<var_list_ids.at(ncount)<<endl;
        cout<<var_vals.at(ncount)<<endl;
        ncount++;
      }
    }
  }

  // In place tests
  cout << "TEST 1 * Value: N(0) variable is 10.01 - "<<get_value(1,"variable1", NEURON)<<endl;
  assert(get_value(1,"variable1", NEURON) == 10.01); cout<<endl;

  cout << "TEST 2 * Value: Syn(0) isyn is 10.9 - "<<get_value(1,"isyn", SYNAPSE)<<endl;
  assert(get_value(1,"isyn", SYNAPSE) == 10.9); cout<<endl;

  cout << "TEST 3 * Value: Get neuron pair : PRE is 1 - ";
  assert(pre_neuron.at(0)==1);

  cout <<" POST is 2 - ";
  assert(post_neuron.at(0)==2);cout<<endl;
  
  stream1.close();
  stream2.close();
  return 0;
}
