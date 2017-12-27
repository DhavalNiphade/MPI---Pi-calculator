#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <mpi.h>

// Initialize limits
#define N 1E8
#define MULT 1E-8

int main(int argc, char* argv[]){
  int rank,i,size,begin,result=0,sum=0;
  long double x, y, pi=0.0;

  MPI_Init(&argc,&argv);

  // Initialize rank and size for all processes
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  begin = MPI_Wtime();


  srand((int)time(0));

  // If the generated point is inside the circle
  for(i=rank ; i<N ; i+=size){
    x = rand()/(RAND_MAX + 1.0);
    y = rand()/(RAND_MAX + 1.0);
    if(x*x+y*y<1.0)
      result++;
  }
  
  // Reduce into sum for the total number of
  MPI_Reduce(&result,&sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

  // Synchronize all the processes
  MPI_Barrier(MPI_COMM_WORLD);

  if(rank==0){
    pi = 4*MULT*sum;
    printf("\nNumber of processes = %d",size);
    printf("\nPI = %0.10f\n",pi);
  }

  MPI_Finalize();
  return 0;
}
