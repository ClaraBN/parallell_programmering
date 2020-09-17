# include <thread>
# include <iostream>
# include <stdio.h>
# include <math.h>
# include <future>
# include <string.h>
# include <chrono>

using namespace std;

void findPrime(volatile int *array, int endSeed, int min, int max){
    //int unmarked=0;
    for(int seedEl=2; seedEl <= endSeed; seedEl++){
        if(seedEl>0){
            for(int chunkEl = min; chunkEl <= max; chunkEl++){
                if(chunkEl%seedEl==0 && chunkEl>0){
                    array[chunkEl-1]=-chunkEl;
                };
            };
        };
    };
};

int main(int argc, char *argv[]) {
    
  if (argc != 3) {
    char h[] = {'-','h'};
    if (strcmp(argv[1], h) == 0) {
      printf("%s\n","First argument is which integer to compute the primes to, the second argument is number of threads");
      return 0;
    };
    printf("%s\n", "invalid input. try -h for help");
    return 0;
  };

  int max = stoi(argv[1]);
 
  int sqrtMax = int(sqrt(max));
  volatile int seed[max];
  for(int i = 0; i < max; i++){
      seed[i] = i+1;
      // printf("Seed: %d\n", seed[i]);
  };

  seed[0]=-1;
  int unmarked = seed[1];
  int nextUnmarked = 0;
  while (nextUnmarked<=sqrtMax){
      for(int j=pow(unmarked,2); j <= sqrtMax; j++){
          if(j%unmarked == 0){
              seed[j-1]=0-j;
              // printf("%s\n", "hej!");
          };
      };
      bool nextUpdated = 0;
      for(int index = unmarked; index <= sqrtMax; index++){
          if(seed[index]>0){
              nextUnmarked=seed[index];
              unmarked=nextUnmarked;
              nextUpdated = 1;
              // printf("Index: %d\n", index);
              break;
          };
      };
      if(!nextUpdated){
          // printf("%s\n", "då!");
          break;
      };
  };
  int numThreads = stoi(argv[2]);
  thread *threads = new thread[numThreads];
  volatile int *ptr=seed;
  
  int bigChunk = max - sqrtMax;
  int smallChunks = bigChunk/numThreads;
  int lBound =sqrtMax+1;
  int hBound =lBound+smallChunks; 
  for (int threadID = 0; threadID < numThreads; threadID++){
      threads[threadID] = thread(findPrime, ptr, sqrtMax, lBound, hBound);
      lBound=hBound+1;
      if (threadID == numThreads-2){
          hBound=max;
      }else{
          hBound=lBound+smallChunks;
      };
  };
  threads[0].join();
  // delete[] threads;
  //findPrime(ptr, sqrtMax, sqrtMax+1, max);
  // just to check the code
  
  for(int i = 0; i < max; i++){
      if(seed[i]>0){
          printf("Seed: %d\n", seed[i]);
      } 
  };

  return 0;
};