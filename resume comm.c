/*Bloque jusqu'à ce que le message soit reçu*/
//Envoyer
MPI_Send(&jeton, 1, MPI_INT, 1, tag1, MPI_COMM_WORLD); // data, nbData, typeData, dest, tag, destination
//Recevoir
MPI_Recv(&jeton, 1, MPI_INT, 0, tag1, MPI_COMM_WORLD, &sta); //data; nbData, typeData, dest, tag, canal, status
MPI_Probe(MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, &sta); //on ne connait pas le tag du message ==> MPI_ANY_TAG


/*Bloque jusqu'à ce que le message soit envoyé*/
//Synchrone
MPI_Ssend(buf_send, n, MPI_BYTE, vois, 0, MPI_COMM_WORLD);
MPI_Recv(buf_recv, n, MPI_BYTE, vois, 0, MPI_COMM_WORLD, &sta);


/*Non bloquant*/
//Buffer
char *buff_mpi;
buff_size = n + MPI_BSEND_OVERHEAD;
buff_mpi = malloc(buff_size);
MPI_Bsend(buf_send, n, MPI_BYTE, vois, 0, MPI_COMM_WORLD);
MPI_Recv(buf_recv, n, MPI_BYTE, vois, 0, MPI_COMM_WORLD, &sta);
MPI_Buffer_detach(&buff_mpi, &buff_size);
free(buff_mpi);


//status
sta.MPI_TAG // obtenir le tag 
sta.MPI_SOURCE // obtenir le rang 
MPI_Get_count(&sta, MPI_DOUBLE, &nreal); //status, typeData, nb reels