/*Mandatory*/
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

/*Image*/
#include "FreeImage.h"
#include "kernel.h"



int main(int argc, char **argv) {


    /*Base*/
    int mpirank, mpisize, len;
    pid_t pid;
    char str_host[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv); // Mandatory

    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank); // processus rank
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize); // number of processus

    MPI_Get_processor_name(str_host, &len); // processus name
    pid = getpid(); // processus
    printf("%d, %d, %s, pid=%d\n", mpirank, mpisize, str_host, pid);

    /*Base++*/
    if (mpirank == 0){// if in processus 0
        // Après cet appel, src est rempli
        read_image_from_file(PathName, &src);
    }
    // all processus share the same var
    MPI_Bcast(&src.width , 1, MPI_INT, 0, MPI_COMM_WORLD); // var, nb var, type, rank of sharing processus, destination
    MPI_Bcast(&src.height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&src.pitch , 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&src.bpp   , 1, MPI_INT, 0, MPI_COMM_WORLD);

    // local copy 
    dst.width  = src.width ;
    dst.height = src.height;
    dst.pitch  = src.pitch ;
    dst.bpp    = src.bpp   ;

    // Calcul de dst.sz_in_bytes et allocation de dst.buf à partir des paramètres dst.width, dst.height, ...
    allocate_image_from_parameters(&dst);

    /**************************/
    /* Traitement sur l'image */
    /**************************/
    int c = 10*mpirank; // TODO : affecter la valeur du décalage
    for (int y = 0; y < src.height; y++)
    {
        for (int x = 0; x < src.width; x++)
        {
        int id = ((y * src.width) + x);
        slide_effect(id, &dst, &src, c);
        }
    }


  /**************************/
  /*Distribution de 1/mpisze de l'image à chaque processus
  /**************************/
  MPI_Scatter(
      src.buf, dst.sz_in_bytes, MPI_BYTE, // pointeur tab origine, taille élément envoyer, type d'élement
      dst.buf, dst.sz_in_bytes, MPI_BYTE, // pointeur vers endroit de stockage, nb d'élément
      /*root=*/0, MPI_COMM_WORLD
      );


    MPI_Finalize();

    return 0;
}

