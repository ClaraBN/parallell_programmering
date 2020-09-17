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

  int unmarked = seed[1];
  int nextUnmarked = 0;
  while (nextUnmarked<=sqrtMax){
      for(int j=pow(unmarked,2); j <= sqrtMax; j++){
          if(j%unmarked == 0){
              seed[j-1]=0-j;
              printf("%s\n", "hej!");
          };
      };
      bool nextUpdated = 0;
      for(int index = unmarked; index <= sqrtMax; index++){
          if(seed[index]>0){
              nextUnmarked=seed[index];
              unmarked=nextUnmarked;
              nextUpdated = 1;
              printf("Index: %d\n", index);
              break;
          };
      };
      if(!nextUpdated){
          printf("%s\n", "då!");
          break;
      };
  };
  // just to check the code
    for(int i = 0; i < sqrtMax; i++){
      printf("Seed%d: %d\n", i, seed[i]);
  };

  return 0;
};