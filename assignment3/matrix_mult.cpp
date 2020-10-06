# include <iostream>
# include <stdio.h>
# include <math.h>
# include <string.h>
# include <omp.h>
# include <chrono>

// compile using export OMP_NUM_THREADS=4


using namespace std;


// creates an N times N array containing values from 0 to 9
static int ** create_array(int N) {
  int **array = new int*[N];
  int i, j;
  int k = 0;
  
  for (i = 0; i < N ; i++){
    array[i] = new int[N];
    for (j = 0; j < N; j++){
      array[i][j] = k%10;
      k++;
    };
  };
  return array;
};


// creates an N times N array containing zeroes
void zero_array(int **array, int N){
    for (int i = 0; i < N; i++){
    for (int j = 0; i< N; i++){
      array[i][j] = 0;
    };
  };
};


// deallocates an array of size N
void delete_array(int **array, int N) {
  for (int  i = 0; i < N; i++) {
    delete [] array[i];
  };
  delete [] array;
};



int main(int argc, char *argv[]){
  if (argc != 2) {
    char h[] = {'-','h'};
    if (strcmp(argv[1], h) == 0) {
      printf("%s\n","first and only argument is the dimension of matrices)");
      return 0;
    };
    printf("%s\n", "invalid input. try -h for help");
    return 0;
  };
  
  int dim = stoi(argv[1]);


  // creates one matrix factor filled with values form 0 to 9 and initialize result vector
  int **a1 = create_array(dim);
  int **result = create_array(dim);

  // the other matrix factor for the multiplication. This is the same matrix as a1 but can be changed.
  int **a2 = a1;
  



  //*******************************************\\
  //**************   case i   *****************\\
  //*******************************************\\

  // set result matrix to zero matrix
  zero_array(result, dim);
  
  // start timing for case i
  auto start_time_1 = chrono::system_clock::now();


  //start parallel region
#pragma omp parallel
  {
    
    //parallelize first loop but not the others
#pragma omp for
    for (int i = 0; i < dim; i++) {
      for (int j = 0; j < dim; j++) {
	for (int k = 0; k < dim; k++) {
	  result[i][j] += a1[i][k]*a2[k][j];
	};
      };
    };
  }

  // stop timing for case i
  chrono::duration<double> duration_1 = (chrono::system_clock::now() - start_time_1);

  //print result
  printf("Matrix multiplication case i: dimension of matrix: %d, runtime: %f \n",dim, duration_1.count());


  //*********************************************\\
  //*************** case ii *********************\\
  //*********************************************\\
  

  // set result matrix to zero matrix
  zero_array(result, dim);

  
  // start timing for case ii
  auto start_time_2 = chrono::system_clock::now();


  //start parallel region
    #pragma omp parallel
  {
    //parallelize the first two loops but not the third
#pragma omp for collapse(2)
    for (int i = 0; i < dim; i++) {
      for (int j = 0; j < dim; j++) {
	for (int k = 0; k < dim; k++) {
	  result[i][j] += a1[i][k]*a2[k][j];
	};
      };
    };
  }

  // stop timing for case ii
  chrono::duration<double> duration_2 = (chrono::system_clock::now() - start_time_2);

  // print result
  printf("Matrix multiplication case ii: dimension of matrix: %d, runtime: %f \n",dim, duration_2.count());


  //****************************************************\\
  //******************** case iii **********************\\
  //****************************************************\\


  // set result matrix to zero matrix
  zero_array(result, dim);


  // start timing for case iii
  auto start_time_3 = chrono::system_clock::now();

  // start parallel region
#pragma omp parallel
  {

    //parallelize all three loops
#pragma omp for collapse(3)
    for (int i = 0; i < dim; i++) {
      for (int j = 0; j < dim; j++) {
	for (int k = 0; k < dim; k++) {
	result[i][j] += a1[i][k]*a2[k][j];
	};
      };
    };
  }

  // start timing for case iii
  chrono::duration<double> duration_3 = (chrono::system_clock::now() - start_time_3);

  // print results
  printf("Matrix multiplication case iii: dimension of matrix: %d, runtime: %f \n",dim, duration_3.count());



  // deallocate arrays
  delete_array(a1, dim);
  delete_array(result, dim);

  
  return 0;
};
