# include <thread>
# include <iostream>
# include <stdio.h>
# include <math.h>
# include <future>
# include <string.h>
# include <chrono>
# include <vector>
# include <omp.h>

using namespace std;

vector<int> findPrime(vector<int> seed, int min, int max){
  vector<int> chunk;

  // creates vector of all numbers
  for (int i = min; i <= max; i++){
    chunk.push_back(i);
  };

  // mark the non-primes
  for(unsigned int i = 0; i < seed.size(); i++){
    for (unsigned int j = 0; j < chunk.size(); j++){
      if (chunk[j]%seed[i] == 0 && chunk[j]>0){
	chunk[j] = -chunk[j];	
      };
    };
  };

  // initiate final solution vector
  vector<int> realchunk;

  // save only the primes
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
  #define NUM_THREADS numThreads

  // initialize Max for sequential part of the algorithm, the seed vector containing all the elements, and the vector containing the primes. 
  int sqrtMax = int(sqrt(max));
  vector<int> seed;
  vector<int> realseed;


  //making the seed vector
  for(int i = 0; i < sqrtMax; i++){
    seed.push_back(i+1);
  };

  // initializing variables for the sequential part of the algorithm
  seed[0]=-1;
  int unmarked = seed[1];
  int nextUnmarked = 0;

  // initializing variables for the parallel part of the algorithm
  int bigChunk = max - sqrtMax;
  int smallChunks = bigChunk/numThreads;

  // start measuring time
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

  
  // clean up the seed vector
  for (int i = 0; i < sqrtMax; i++){
    if (seed[i] > 0){
      realseed.push_back(seed[i]);
    };
  };

  
  // set the number of threads
  omp_set_num_threads(NUM_THREADS);

  //copy the realseed vector for threads to read
  vector<int> realseedCopy = realseed;

  
  //start parallel region
  #pragma omp parallel 
  {
    //find id for the thread
    int id = omp_get_thread_num();

    //define bounds that this thread should compute primes between
    int hBound;
    int lBound = (sqrtMax +1) + (smallChunks+1)*id;
    if (id == (NUM_THREADS - 1)) {
      hBound = max;
    } else {
    hBound = lBound + smallChunks;
    };

    //calculate the primes
    vector<int> resultVector = findPrime(realseedCopy, lBound, hBound);


    // critical section for adding the computed primes into realseed
    #pragma omp critical 
    realseed.insert(realseed.end(), resultVector.begin(), resultVector.end());
  }

  // stop timing
  chrono::duration<double> duration = (chrono::system_clock::now() - start_time);

  // print results
  printf("Number of primes: %lu\nRuntime: %f\n",realseed.size(), duration.count());
  
  return 0;
};
