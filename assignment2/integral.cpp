# include <thread>
# include <iostream>
# include <stdio.h>
# include <math.h>
# include <future>
# include <string.h>
# include <chrono>

using namespace std;


double compute(double x0, double x1, double tpt) {

  double area;
  double areaTemp;

  double inc = (x1 - x0)/tpt;
  double x = x0;
  double y;

  for (int i = 0; i < tpt; i++) {
    x = x + inc;
    y = 4/(1 + pow(x,2));
    areaTemp = inc*y;
    area = area + areaTemp;
  };

  return area;
};





int main(int argc, char *argv[]) {

  if (argc != 3) {
    char h[] = {'-','h'};
    if (strcmp(argv[1], h) == 0) {
      printf("%s\n","First argument is the number of threads (integer), the second argument is number of trapezes(integer) and trapezes should be a multiple of threads");
      return 0;
    };
    printf("%s\n", "invalid input. try -h for help");
    return 0;
  };


  
  int numThreads = stoi(argv[1]);
  int trapz = stoi(argv[2]);

  double trapzPerThread = trapz/numThreads;
  double intervalStep = 1/double(numThreads);
  double interval[int(trapzPerThread)];
  interval[0] = 0;
  
  for (int i = 1; i <= numThreads; i++) {
    interval[i] = interval[i-1] + intervalStep;
    //printf("%f\n", interval[i]);

  };


  //thread *threads = new thread[numThreads];

  future<double> futures[numThreads];
  auto start_time = chrono::system_clock::now();
  for (int j = 0; j < numThreads; j++){
    futures[j] = async(compute, interval[j], interval[j+1], trapzPerThread);
  };

  double result = 0;
  for (int j = 0; j < numThreads; j++){
    double n = futures[j].get();
    // cout << n << "\n";
    result = result + n;
    // cout << result << "\n";
  };


    //for (int threadID = 0; threadID < numThreads; threadID++){
    //threads[threadID] = thread(compute, interval[threadID], interval[threadID+1], trapzPerThread);
    //};

  //double result = threads[0].get();

  //calculating the time
  chrono::duration<double> duration = (chrono::system_clock::now() - start_time);
  cout << "Result: " << result << "\n" << "Duration: " << duration.count() << "\n";

  //threads[0].join();

  //delete[] threads;
  return 0;

    };
