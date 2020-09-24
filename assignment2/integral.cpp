# include <thread>
# include <iostream>
# include <stdio.h>
# include <math.h>
# include <future>
# include <string.h>
# include <chrono>
# include <random>

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

  // saving the inputs as variables 
  int numThreads = stoi(argv[1]);
  int trapz = stoi(argv[2]);

  // making all the threads
  future<double> futures[numThreads];


        /*        ************************         */
        /*           EQUAL DISTRIBUTION            */
        /*        ************************         */


  // Calculate how to distrubute trapz per threads
  double trapzPerThread = trapz/numThreads;
  double intervalStep = 1/double(numThreads);
  double interval[int(trapzPerThread)];
  
  // saving the interval into an array
  interval[0] = 0;
  for (int i = 1; i <= numThreads; i++) {
    interval[i] = interval[i-1] + intervalStep;
  };

  // starting timer
  auto start_time = chrono::system_clock::now();

  // for each thread run "compute"
  for (int j = 0; j < numThreads; j++){
    futures[j] = async(compute, interval[j], interval[j+1], trapzPerThread);
  };

  // sum all the threads results into a result 
  double result = 0;
  for (int j = 0; j < numThreads; j++){
    double n = futures[j].get();
    result = result + n;
  };

  //calculating the runtime and write out tut to the terminal
  chrono::duration<double> duration = (chrono::system_clock::now() - start_time);
  cout << "even distribution: \n" << "Result: " << result << "\n" << "Duration: " << duration.count() << endl;


  /*   *************************   */
  /*      RANDOM DISTRIBUTION      */
  /*   *************************   */


  // Calculate how to distrubute trapz per threads
      

  // saving the interval into an array
  int intervals[numThreads+1];
  intervals[0] = 0;
  int sum = 0;
  int trapzMax = trapz-numThreads+1;
  
  for (int i = 1; i <= numThreads; i++) {
    double perc = ( rand()%100 )/100.0 + 0.01; // in the range 0.01 to 1
    int step = int(perc*(trapzMax-sum));
    printf("\nStep: %d\n",step);
    printf("Sum: %d\n", sum);

    if(i == numThreads){
      intervals[i] = trapz - sum;
      sum = sum + (trapz - sum);
      printf("%d: %d\n\n", i, intervals[i]);

    }else if ((sum+step) < trapzMax){
      intervals[i] = intervals[i-1] + step;
      sum = sum + (intervals[i-1] + step);
      printf("%d: %d\n\n", i, intervals[i]);

    }else{
      intervals[i] = 1;
      sum = sum + 1;
      printf("%d: %d\n\n", i, intervals[i]);
    };
  };
  // starting timer
  //auto start_time = chrono::system_clock::now();

  // for each thread run "compute"

  // sum all the threads results into a result 

  //calculating the runtime and write out tut to the terminal





  //thread *threads = new thread[numThreads];
  //threads[0].join();
  //delete[] threads;
  return 0;

  };
