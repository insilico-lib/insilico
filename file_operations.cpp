#include <fstream>
#include <iostream>
#include <cstring>
#include <stdlib.h>

using namespace std;

int main(){
  ifstream stream1("nsets.conf"), stream2("ssets.conf");
  string str;
  
  cout<<"nsets\n";
  while(getline(stream1,str) > 0){
    cout<<str<<endl;
  }
  
  cout<<"ssets\n";
  while(getline(stream2,str) > 0){
    cout<<str<<endl;
  }
  
  return 0;
}
  
