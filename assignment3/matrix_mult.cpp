# include <iostream>
# include <stdio.h>
# include <math.h>
# include <string.h>
# include <omp.h>
# include <chrono>

// compile using   export OMP_NUM_THREADS=4


using namespace std;

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


void zero_array(int **array, int N){
    for (int i = 0; i < N; i++){
    for (int j = 0; i< N; i++){
      array[i][j] = 0;
    };
  };
};



void delete_array(int **array, int N) {
  for (int  i = 0; i < N; i++) {
    delete [] array[i];
  };
  delete [] array;
};



// static void free_array(int ** array, int N) {
// 	int i;
// 	for (i = 0 ; i < N ; i++){
// 		free(array[i]);
// 	};
// 	free(array);
// };




int main(int argc, char *argv[]){
  if (argc != 2) {
    char h[] = {'-','h'};
    if (strcmp(argv[1], h) == 0) {
      printf("%s\n","first argument is dimension of matrices)");
      return 0;
    };
    printf("%s\n", "invalid input. try -h for help");
    return 0;
  };
  
  int dim = stoi(argv[1]);


  // create matrices
  int **a1 = create_array(dim);
  int **a2 = a1;
  int **result = create_array(dim);

  zero_array(result, dim);

			     

  // cout << "a1: " << "\n";
  // for (int i = 0; i < dim; i++){
  //   for (int j = 0; j < dim; j++) {
  //     cout << a1[i][j] << " ";
  //   };
  //   cout << "\n";
  // };
  
  // cout << "a2: " << "\n";
  // for (int i = 0; i < dim; i++){
  //   for (int j = 0; j < dim; j++) {
  //     cout << a2[i][j] << " ";
  //   };
  //   cout << "\n";
  // };
  
  
  // multiply matrices


 
    auto start_time_1 = chrono::system_clock::now();
    
#pragma omp parallel
  {
#pragma omp for
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      for (int k = 0; k < dim; k++) {
	//printf("before: %d, k: %d\n", result[i][j],k);
	//printf("result: %d, a1: %d, a2 %d ", result[i][j], a1[i][k], a2[k][j]);
	result[i][j] += a1[i][k]*a2[k][j];
	//printf("resultresutl: %d \n", result[i][j]);
	//printf("after: %d, k: %d\n", result[i][j],k);
      };
      //printf("row: %d, column: %d, entry: %d\n",i,j,result[i][j]);
    };
  };
  }

  // cout << "result: " << "\n";
  // for (int i = 0; i < dim; i++){
  //   for (int j = 0; j < dim; j++) {
  //     cout << result[i][j] << " ";
  //   }
  //   cout << "\n";
  // }
  chrono::duration<double> duration_1 = (chrono::system_clock::now() - start_time_1);
  printf("Matrix multiplication case i: dimension of matrix: %d, runtime: %f \n",dim, duration_1.count());



  zero_array(result, dim);

  auto start_time_2 = chrono::system_clock::now();

    
    #pragma omp parallel
  {
#pragma omp for collapse(2)
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      for (int k = 0; k < dim; k++) {
	//printf("before: %d, k: %d\n", result[i][j],k);
	//printf("result: %d, a1: %d, a2 %d ", result[i][j], a1[i][k], a2[k][j]);
	result[i][j] += a1[i][k]*a2[k][j];
	//printf("resultresutl: %d \n", result[i][j]);
	//printf("after: %d, k: %d\n", result[i][j],k);
      };
      //printf("row: %d, column: %d, entry: %d\n",i,j,result[i][j]);
    };
  };
  }

  // cout << "result: " << "\n";
  // for (int i = 0; i < dim; i++){
  //   for (int j = 0; j < dim; j++) {
  //     cout << result[i][j] << " ";
  //   };
  //   cout << "\n";
  // };


    chrono::duration<double> duration_2 = (chrono::system_clock::now() - start_time_2);
    printf("Matrix multiplication case ii: dimension of matrix: %d, runtime: %f \n",dim, duration_2.count());


    zero_array(result, dim);
  
  auto start_time_3 = chrono::system_clock::now();
#pragma omp parallel
  {
#pragma omp for collapse(3)
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      for (int k = 0; k < dim; k++) {
	//printf("before: %d, k: %d\n", result[i][j],k);
	//printf("result: %d, a1: %d, a2 %d ", result[i][j], a1[i][k], a2[k][j]);
	result[i][j] += a1[i][k]*a2[k][j];
	//printf("resultresutl: %d \n", result[i][j]);
	//printf("after: %d, k: %d\n", result[i][j],k);
      };
      //printf("row: %d, column: %d, entry: %d\n",i,j,result[i][j]);
    };
  };
  }

  // cout << "result: " << "\n";
  // for (int i = 0; i < dim; i++){
  //   for (int j = 0; j < dim; j++) {
  //     cout << result[i][j] << " ";
  //   };
  //   cout << "\n";
  // };

  chrono::duration<double> duration_3 = (chrono::system_clock::now() - start_time_3);
  printf("Matrix multiplication case iii: dimension of matrix: %d, runtime: %f \n",dim, duration_3.count());


  
  delete_array(a1, dim);
  delete_array(result, dim);
  return 0;
};
