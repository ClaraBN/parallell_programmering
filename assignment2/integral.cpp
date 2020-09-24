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

  // Calculating the intervals needed between 0 and 1 depending on munber of threads
  double intervalStep = 1/double(numThreads);


        /*        ************************         */
        /*           EQUAL DISTRIBUTION            */
        /*        ************************         */


  // Calculate how to distrubute trapz per threads 
  double trapzPerThread = trapz/numThreads;
  
  // saving the interval into an array
  double interval[int(trapzPerThread)];
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
  cout << "Even distribution: \n" << "Result: " << result << "\n" << "Duration: " << duration.count() << endl;


  /*   *************************************   */
  /*      RANDOM DISTRIBUTION OF TRAPEZES      */
  /*   *************************************   */


  // Calculate how to distrubute trapz per threads "randomly-ish"
  // each thread will have at least one trapeze
  double trapezes[numThreads];
  double sum = 0;
  double trapzMax = trapz-numThreads+1;
  
  for (int i = 0; i < numThreads; i++) {
    double perc = ( rand()%10 )/100.0 + 0.01; // in the range 0.01 to 0.1
    double step = int(perc*(trapz-sum));

    if(i == numThreads){
      trapezes[i] = trapz;

    }else if ((sum+step) < trapzMax){
      trapezes[i] = sum + step;
      sum = sum + step;

    }else{
      trapezes[i] = 1.0;
      sum = sum + 1.0;
    };
    // printf("\ntrapezes %d: %f\n",i, trapezes[i]);
  };      

  // saving the intervals into an array
  interval[0] = 0;
  for (int i = 1; i <= numThreads; i++) {
    interval[i] = interval[i-1] + intervalStep;
  };
  // starting timer
  auto start_time2 = chrono::system_clock::now();

  // for each thread run "compute"
  for (int j = 0; j < numThreads; j++){
    futures[j] = async(compute, interval[j], interval[j+1], trapezes[j]);
  };

  // sum all the threads results into a result 
  result = 0;
  for (int j = 0; j < numThreads; j++){
    double n = futures[j].get();
    result = result + n;
  };

  //calculating the runtime and write out to the terminal
  chrono::duration<double> duration2 = (chrono::system_clock::now() - start_time2);
  cout << "\nRandom distribution trapezes: \n" << "Result: " << result << "\n" << "Duration: " << duration2.count() << endl;


 /*   *****************************************************   */
 /*      ALL IN ONE, DONT CARE- DISTRIBUTION OF TRAPEZES      */
 /*   *****************************************************   */


  // All trapezes in one thread in the beginning, almost. 
  // Each thread will have at least one trapeze
  sum = 0;
  trapzMax = trapz-numThreads+1;
  trapezes[0]=trapzMax;
  printf("\ntrapezes %d: %f\n",0, trapezes[0]);

  for (int i = 1; i < numThreads; i++) {
    trapezes[i] = 1.0;
    printf("trapezes %d: %f\n",i, trapezes[i]);
  }; 

  // saving the intervals into an array
  interval[0] = 0;
  for (int i = 1; i <= numThreads; i++) {
    interval[i] = interval[i-1] + intervalStep;
  };

  // starting timer
  //auto start_time2 = chrono::system_clock::now();

  // for each thread run "compute"
  for (int j = 0; j < numThreads; j++){
    futures[j] = async(compute, interval[j], interval[j+1], trapezes[j]);
  };

  // sum all the threads results into a result 
  result = 0;
  for (int j = 0; j < numThreads; j++){
    double n = futures[j].get();
    result = result + n;
  };

  //calculating the runtime and write out to the terminal
  //chrono::duration<double> duration2 = (chrono::system_clock::now() - start_time2);
  cout << "\nRandom distribution trapezes: \n" << "Result: " << result << "\n" << "Duration: " << duration2.count() << endl;


  //thread *threads = new thread[numThreads];
  //threads[0].join();
  //delete[] threads;
  return 0;

  };
