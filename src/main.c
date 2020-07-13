#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#define BUFFER_SIZE (100) // Tamaño del array buffer

int main()
{
    // Nombre del buffer en memoria compartida (dirección) 
    const char *buffer_name = "/BUFF";

    //File descriptor de la memoria compartida
    int shm_fd;

    // puntero al buffer en memoria compartida 
    void *buffer_ptr;

    /* Crear el objeto de memoria compartida */
    shm_fd = shm_open(buffer_name, O_CREAT | O_RDRW, 0666);

    if (shm_fd == -1)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }     

    /* Configurar el tamaño del objeto en memoria compartida */
    ftruncate(shm_fd, size);

    /* Mapear memoria del objeto compartido en memoria */
    buffer_ptr = mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    if (buffer_ptr == MAP_FAILED)
    {
        close(shm_fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    int i;

    // Inicializar la memoria compartida a '\0'
/*    for(i=0;i<BUFFER_SIZE; i++) {
       buffer_ptr[i] = '\0';
    }*/
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
   


   // bool wait;
    //bool signal;
//    int active_producers = 0; //Productores activos
//    int active_consumers = 0; //Consumidores activos
    int numer_Choice = 0;
    int waiting_time;
    while(numer_Choice != 4){
        puts("/========================/");
        puts("          Menu            ");
        puts("/========================/");
        puts("1. Productor");
        puts("2. Consumidor");
        puts("3. Finalizacion");
        puts("4. Exit");
        printf(">>\n");
        scanf("%d", &numer_Choice);
        switch(numer_Choice){
        case 1:
            puts("Agregar nuevo Productor");
            puts("Nombre del Buffer");
            scanf( "%s" , buffer_name );
            puts("Tiempo de espera");
            scanf("%d", &waiting_time);
            break;
        case 2:
            puts("Agregar nuevo Consumidor");
            puts("Nombre del Buffer");
            scanf( "%s" , buffer_name );
            puts("Tiempo de espera");
            scanf("%d", &waiting_time);
            break;
        case 3:
            printf("\n");
            printf("%s",process_name_producer);
            printf("\n");
            printf("%s",process_name_consumer);
            printf("\n");
            printf("%d", waiting_time);
            printf("\n");
            break;
        default:
            puts("Exit");
            numer_Choice = 4;
            break;
        }

    }
    return 0;
}
