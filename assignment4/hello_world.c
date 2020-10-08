#include <stdio.h>
#include "mpi.h"
int main(int argc, char *argv[])
{
char name[MPI_MAX_PROCESSOR_NAME];
int rank, size, len;

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Get_processor_name(name, &len);
printf("I am %d of %d on %s\n", rank, size, name);
MPI_Finalize();
return 0;
}
/*
#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    int rank, nprocs, len;
    char name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(name, &len);
    printf("Hello, world.  I am %d of %d on %s\n", rank, nprocs, name);fflush(stdout);
    MPI_Finalize();
    return 0;
}
*/