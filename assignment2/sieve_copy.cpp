# include <thread>
# include <iostream>
# include <stdio.h>
# include <math.h>
# include <future>
# include <string.h>
# include <chrono>
# include <list>

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
  int numThreads = stoi(argv[2]);
  int sqrtMax = int(sqrt(max));
  
  // making the complete list
  list<int> seed;
  for (int i = 0; i < max; i++){
      seed.push_back(i+1);
  };
  // setting the first value in seed to -1
  seed.front() = -1;
  // made an iterator to interate through the list
  list<int>::iterator it;
  // it is pointed at the beginning of the list
  it = seed.begin();
  // it is now pointing to the second value in the list
  advance(it, 1);
  int unmarked = *it;
  int nextUnmarked = 0;
  
  thread *threads = new thread[numThreads];
  volatile int *ptr=seed;
  
  int bigChunk = max - sqrtMax;
  int smallChunks = bigChunk/numThreads;
  int lBound =sqrtMax+1;
  int hBound =lBound+smallChunks; 
  
  // actually calculating the primes
  auto start_time = chrono::system_clock::now();
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

  for (int threadID = 0; threadID < numThreads; threadID++){
      threads[threadID] = thread(findPrime, ptr, sqrtMax, lBound, hBound);
      lBound=hBound+1;
      if (threadID == numThreads-2){
          hBound=max;
      }else{
          hBound=lBound+smallChunks;
      };
  };

  for(int i = 0; i<numThreads; i++){
      threads[i].join();
  }
  
  //calculating the time
  chrono::duration<double> duration = (chrono::system_clock::now() - start_time);
  // delete[] threads;
  //findPrime(ptr, sqrtMax, sqrtMax+1, max);
  // just to check the code
  
  int primeNumbers=0;
  for(int i = 0; i < max; i++){
      if(seed[i]>0){
          primeNumbers++;   
      } 
  };

  printf("Number of primes: %d\nRuntime: %f\n",primeNumbers, duration.count());
  return 0;
};