#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

int main() {
    int my_rank, comm_sz;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    int* my_values = malloc(2 * sizeof(int));
    my_values[0] = my_rank;
    my_values[1] = -1;

    printf("1: process %d has value %d\n", my_rank, my_values[0]);

    if (comm_sz % 2 == 0) {
        if (my_rank % 2 == 0) {
            if (my_rank == 0) {
                MPI_Ssend(&my_rank, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(my_values, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Ssend(&my_rank, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
                MPI_Ssend(&my_rank, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(my_values, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&my_values[1], 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } else {
            if (my_rank == comm_sz - 1) {
                MPI_Recv(my_values, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Ssend(&my_rank, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
            } else {
                MPI_Recv(my_values, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&my_values[1], 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Ssend(&my_rank, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
                MPI_Ssend(&my_rank, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        if (my_rank % 2 == 0) {
            if (my_rank == 0) {
                MPI_Ssend(&my_rank, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(my_values, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else if (my_rank == comm_sz - 1) {
                MPI_Ssend(&my_rank, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
                MPI_Recv(my_values, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Ssend(&my_rank, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
                MPI_Ssend(&my_rank, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(my_values, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&my_values[1], 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } else {
            MPI_Recv(my_values, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&my_values[1], 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Ssend(&my_rank, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
            MPI_Ssend(&my_rank, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    if (my_values[1] != -1) {
        printf("2: process %d received vals %d, %d\n", my_rank, my_values[0], my_values[1]);
    } else {
        printf("2: process %d received val %d\n", my_rank, my_values[0]);
    }

    free(my_values);

    MPI_Finalize();

    return 0;
}
