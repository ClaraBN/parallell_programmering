#include <iostream>
#include <thread>
#include <chrono>
#include "test.h"


int main () {
  int T[6]={1,2,4,8,16,32};
  int N[6]={1,2,4,8,16,32};

  for(int i=0; i<6; i++){
    for(int j=0; i<6; i++){
      int k=performance(T[i], N[j]);
    };
  };
  return(0);
};
