#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/shm.h> 
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <semaphore.h>

//#define OBJECT_SIZE (sizeof(char[100][20]) + sizeof(sem_t) + 3*sizeof(int)) // Tamaño del objeto en memoria compartida

const char *sem_name = "SEM_GARFIELD"; // Nombre del semáforo (variable constante)

void create_variables_in_sharedmemory(char *buffer_name) {
    //bool end_signal;
//    int active_producers = 0; //Productores activos
//    int active_consumers = 0; //Consumidores activos
}

/*    // Nombre del buffer en memoria compartida (dirección) 
    const char *buffer_name = "/BUFF";

    //File descriptor de la memoria compartida
    int shm_fd;

    // puntero al buffer en memoria compartida 
    void *buffer_ptr;

    // Crear el objeto de memoria compartida 
    shm_fd = shm_open(buffer_name, O_CREAT | O_RDRW, 0666);

    if (shm_fd == -1)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }     

    // Configurar el tamaño del objeto en memoria compartida
    ftruncate(shm_fd, BUFFER_SIZE);

    // Mapear memoria del objeto compartido en memoria
    buffer_ptr = mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    if (buffer_ptr == MAP_FAILED)
    {
        close(shm_fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    int i;

    // Inicializar la memoria compartida a '\0'
//    for(i=0;i<BUFFER_SIZE; i++) {
  //     buffer_ptr[i] = '\0';
  //  }
    memset( buffer_ptr, '\0', SIZE );
    
    // Liberar la memoria mapeada (liberar el buffer)
    if (munmap(buffer_ptr, BUFFER_SIZE) == -1)
    {
        close(shm_fd);
        perror("Error un-mmapping the file");
        exit(EXIT_FAILURE);
    }

    // Cerrar File.
    close(shm_fd);
   

*/

int main()
{
    char buffer_name[30]; // Nombre del buffer en memoria compartida (dirección) 

    printf(">>\n");
    printf("getpid(): %d", getpid()); // Obtener e imprimir el id del proceso
    printf(">>\n");
    return 0;
}

