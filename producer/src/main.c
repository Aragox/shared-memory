#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/shm.h> 
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <semaphore.h>
  
int new_producer(char *name, int size, int waiting_time) 
{ 
  
    // mensaje a producir/escribir en memoria compartida 
    //char* message = generate_message(); 
  
    // file descriptor de memoria compartida 
    int shm_fd; 
  
    // puntero a objeto en memoria compartida 
    void* ptr; 
  
    // crear el objeto de memoria compartida 
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); 
  
    // configurar el tamaño del objeto en memoria compartida 
    ftruncate(shm_fd, size); 
  
    // mapear memoria de objeto en memoria compartida 
    ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, shm_fd, 0); 

    //Escribir al objeto en memoria compartida
    ptr += sizeof(char[100][20]) + sizeof(sem_t); // Aumenta la cantidad de productores activos

    //strcpy(ptr[100], message); // Depositar mensaje en el buffer

    return 1;
}

int main()
{
    printf(">>\n");
    printf("getpid(): %d", getpid()); // Obtener e imprimir el id del proceso
    printf(">>\n");
    return 0;
} 
