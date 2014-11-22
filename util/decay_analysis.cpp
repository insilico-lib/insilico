#include <iostream>
#include <vector>

using namespace std;

int main() {
  int i = -5, prev_i = -5;
  int t = 0, thresh = 2;
  int state = -10, spike = 0;

  // loop through all the timesteps
  while(t < 100) {
    if(state == -10) { prev_i=i; i++; if(i==5) state=5; }
    else if(state == 5) { prev_i=i; i--; if(i==-10) state=-10; }

    if(prev_i > thresh && i > thresh && prev_i < i && spike < 1) { cout<<prev_i<<" "<<i<<" spike"<<endl; spike++; }
    else {spike = 0;}
    
    t++;
  }    
  return 0;
}
