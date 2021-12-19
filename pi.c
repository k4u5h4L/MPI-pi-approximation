#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#include <time.h>
#define SEED 69420

double getRandomNumber() {
    return ((double) rand()) / RAND_MAX;
}

int main(int argc, char * argv[]) {
    long intervals = 1000000;
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start = MPI_Wtime();

    if (size <= 1) {
        printf("Pass minimum 2 number of processes\nExiting...\n");
        MPI_Finalize();
        exit(1);
    }

    int sq_points = 0, circle_points = 0;
    double pi;
    double x, y;
    double origin_dist;

    int inbuf_sq, inbuf_circle;

    int sq_total_points = 0, circle_total_points = 0;

    // srand(time(0));
    srand(SEED + rank);

    if (rank != 0) {
        for (int i = 0; i < intervals; i++) {
            x = getRandomNumber();
            y = getRandomNumber();

            // Distance between (x, y) from the origin
            origin_dist = sqrt((x * x) + (y * y));

            if (origin_dist<= 1) {
                circle_points++;
            }

            sq_points++;
        }

        MPI_Send(&sq_points, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&circle_points, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        for (int i = 0; i < size - 1; i++) {
            MPI_Recv(&inbuf_sq, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&inbuf_circle, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("From rank %d, points in sq = %d, points in circle = %d\n", i + 1, inbuf_sq, inbuf_circle);

            sq_total_points += inbuf_sq;
            circle_total_points += inbuf_circle;
        }

        printf("\nTotal points received at rank 0: Circle = %d, Sq = %d\n", circle_total_points, sq_total_points);

        pi = (double) 4 * ((double) circle_total_points / (double) sq_total_points);

        printf("\nPi: %.10lf\n", pi);

        double end = MPI_Wtime();

        printf("Time taken: %.10lf\n", end - start);
    }

    MPI_Finalize();
    return 0;
}