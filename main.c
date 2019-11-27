#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "byzantine.h"


int main(int argc, char *argv[]) {
    srand(time(NULL));

    int rank;
    int no_processes;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &no_processes);

    int final_decision = reach_consensus(rank, no_processes);

    printf("Process no [%d] with says consensus was reached with value %d\n", rank, final_decision);

    MPI_Finalize();
    return 0;
}

