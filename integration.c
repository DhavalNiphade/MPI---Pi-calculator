#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>


int main(int argc, char* argv[]) {

    double intervals=1024;
    double pi=acos(-1.0),result,high=pi/2;
    //printf("\n%f",pi);

    double sinx=0.0,cosx=0.0,deg=0.0,buf=0.0,rad=0.0;
    int i;
    double sizeOfInterval = high/intervals;
    //printf("\n%f",sizeOfInterval);

    // Initialize MPI
    MPI_Init(&argc,&argv);

    // Get rank and size
    int rank=0,size=0;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    // Calculate the value all integrals in all slave processes
        for(i=rank; i<=intervals; i+=size){
            deg = i*sizeOfInterval;
	    //printf("\ncurrent degree = %f",deg);
            rad = (deg*pi)/180.0;
            //printf("\n%f",rad);
            sinx = sin(rad)/2.0;
            cosx = cos(rad);
	    //printf("\nsin = %f \t cos = %f",sinx,cosx);
            buf = buf + sinx*cosx;
        }
   
	
	//printf("\nBuffer for rank %d  =  %f",rank,buf);

    MPI_Reduce(&buf,&result,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0)
        printf("\nIntegration Result = %0.12f\n",result);

    // End Finalize
    MPI_Finalize();

    return 0;
}

