# include <thread>
# include <iostream>
# include <stdio.h>
# include <math.h>
# include <future>
# include <string.h>
# include <chrono>

using namespace std;

void findPrime(int *arraySeed, int *array){

};

int main(int argc, char *argv[]) {
    
  if (argc != 2) {
    char h[] = {'-','h'};
    if (strcmp(argv[1], h) == 0) {
      printf("%s\n","First argument is the number of threads (integer), the second argument is number of trapezes(integer) and trapezes should be a multiple of threads");
      return 0;
    };
    printf("%s\n", "invalid input. try -h for help");
    return 0;
  };

  // int max = stoi(argv[1]);
  int max = 25;
 
  int sqrtMax = int(sqrt(max));
  int seed[sqrtMax];
  for(int i = 0; i < sqrtMax; i++){
      seed[i] = i+1;
      // printf("Seed: %d\n", seed[i]);
  };

  int k = seed[1];
  k=k+1;
  
  printf("Seed: %d\n k: %d\n", seed[i],k);



  return 0;
};