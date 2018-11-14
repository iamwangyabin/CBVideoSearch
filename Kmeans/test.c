#include "stdint.h" //重点是这一句

#include <stdio.h>
#include <math.h>
/*
void main(argc,argv)
int argc;
char *argv[];
{
    int myid, numprocs;
    int  namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Get_processor_name(processor_name,&namelen);
    fprintf(stderr,"Hello World! Process %d of %d on %s\n",myid,numprocs,processor_name);
    MPI_Finalize();
}*/

//gcc -o test.exe test.c -l msmpi -L "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86\" -I "C:\Program Files (x86)\Microsoft SDKs\MPI\Include\ "

//gcc -o test.exe test.c -l msmpi -L "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -I "C:\Program Files (x86)\Microsoft SDKs\MPI\Include"
int main(){
    return 0;
}