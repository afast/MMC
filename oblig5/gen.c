#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include "mt19937ar.c"

using namespace std;

int main(int argc, char* argv[]) {

  for(int i = 0; i<1000000;i++){
    cout << genrand_real2() << " ";
  }

  return 0;
}
