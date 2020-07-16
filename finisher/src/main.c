#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/shm.h> 
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <semaphore.h>


int main()
{
    printf(">>\n");
    printf("getpid(): %d", getpid()); // Obtener e imprimir el id del proceso
    printf(">>\n");
    return 0;
}
