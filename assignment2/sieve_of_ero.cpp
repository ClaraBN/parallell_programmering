# include <thread>
# include <iostream>
# include <stdio.h>
# include <math.h>
# include <future>
# include <string.h>
# include <chrono>
# include <vector>

using namespace std;

vector<int> findPrime(vector<int> seed, int endSeed, int min, int max){
  vector<int> chunk;

  for (int i = min; i <= max; i++){
    chunk.push_back(i);
  };
  
  for(unsigned int i = 0; i < seed.size(); i++){
    for (unsigned int j = 0; j < chunk.size(); j++){
      if (chunk[j]%seed[i] == 0 && chunk[j]>0){
	chunk[j] = -chunk[j];	
      };
    };
  };

  vector<int> realchunk;
  for (unsigned int i = 0; i < chunk.size(); i++){
    if (chunk[i] > 0){
      realchunk.push_back(chunk[i]);
    };
  };

  return realchunk;
};



int main(int argc, char *argv[]) {
    
  if (argc != 3) {
    char h[] = {'-','h'};
    if (strcmp(argv[1], h) == 0) {
      printf("%s\n","First argument is which integer to compute the primes to, the second argument is number of threads, the integer cannot be lower than 4 or that the number of theads");
      return 0;
    };
    printf("%s\n", "invalid input. try -h for help");
    return 0;
  };

  int max = stoi(argv[1]);
  int numThreads = stoi(argv[2]);
  int sqrtMax = int(sqrt(max));
  
  vector<int> seed;
  vector<int> realseed;


  //making the seed array
  for(int i = 0; i < sqrtMax; i++){
    seed.push_back(i+1);
  };

  seed[0]=-1;
  int unmarked = seed[1];
  int nextUnmarked = 0;
  int bigChunk = max - sqrtMax;
  int smallChunks = bigChunk/numThreads;
  int lBound =sqrtMax + 1;
  int temp = lBound + smallChunks;
  int temp2 = temp + lBound;
  int hBound;
  if (temp2 > max){
    hBound = max;
  } else {
    hBound = temp;
  };


  
  auto start_time = chrono::system_clock::now();

  
  // calculating the seed primes
  while (nextUnmarked<=sqrtMax){
      for(int j=pow(unmarked,2); j <= sqrtMax; j++){
          if(j%unmarked == 0){
              seed[j-1]=0-j;
          };
      };
      bool nextUpdated = 0;
      for(int index = unmarked; index < sqrtMax; index++){
          if(seed[index]>0){
              nextUnmarked=seed[index];
              unmarked=nextUnmarked;
              nextUpdated = 1;
              break;
          };
      };
      if(!nextUpdated){
          break;
      };
  };
  

  // clean the seed vector
  for (int i = 0; i < sqrtMax; i++){
    if (seed[i] > 0){
      realseed.push_back(seed[i]);
    };
  };


  future<vector<int>> futures[numThreads];
  for (int j = 0; j < numThreads; j++){
    futures[j] = async(findPrime, realseed, sqrtMax, lBound, hBound);
    lBound=hBound+1;
    if (j == numThreads-2){
      hBound = max;
    } else {
      hBound = lBound + smallChunks;
    };
  };

  for(int j = 0; j < numThreads; j++){
    vector<int> temp = futures[j].get();
    realseed.insert(realseed.end(), temp.begin(), temp.end());
  };
  chrono::duration<double> duration = (chrono::system_clock::now() - start_time);

  
  printf("Number of primes: %lu\nRuntime: %f\n",realseed.size(), duration.count());
  return 0;


};
