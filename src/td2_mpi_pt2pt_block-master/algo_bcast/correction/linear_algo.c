#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

void linear_bcast(char *buf, int n, int nproc, int rank)
{
    int tag, i;

    tag = 1000;

    if (rank == 0)
    {
        for(i = 1 ; i < nproc ; i++)
        {
            MPI_Ssend(buf, n, MPI_BYTE, i, tag, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(buf, n, MPI_BYTE, /*root=*/0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void fill_buf(char *buf, int n)
{
    int i;
    for(i = 0 ; i < n ; i++)
    {
      buf[i] = (char)((i+i/256)%256);
    }
}

void check_buf(char *buf, int n)
{
    int i;
    for(i = 0 ; i < n ; i++)
    {
      if (buf[i] != (char)((i+i/256)%256))
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
}

#define NITER 100

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    int rank, iter, nproc;
    char *buf;
    double tend, tbeg;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    buf = (char*)malloc(n*sizeof(char));

    if (rank == /*root=*/0)
    {
        printf("%d bytes\n", n);
        fflush(stdout);
        fill_buf(buf, n);
    }

    tbeg = MPI_Wtime();

    for(iter = 0 ; iter < NITER ; iter++)
    {
        linear_bcast(buf, n, nproc, rank);
    }

    tend = MPI_Wtime();

    check_buf(buf, n);
    if (rank == 0)
    {
        printf("Telaps for %d bcast = %.3e s\n", NITER, tend - tbeg);
    }

    free(buf);

    MPI_Finalize();
    return 0;
}

