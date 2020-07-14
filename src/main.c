#include "producer.h"

#define OBJECT_SIZE (sizeof(char[100][20]) + sizeof(sem_t) + 3*sizeof(int)) // Tamaño del objeto en memoria compartida

const char *sem_name = "SEM_GARFIELD"; // Nombre del semáforo (variable constante)

void create_variables_in_sharedmemory(char *buffer_name) {
    //bool end_signal;
//    int active_producers = 0; //Productores activos
//    int active_consumers = 0; //Consumidores activos
}

int main()
{
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
   // bool wait;

    int numer_Choice = 0;
    char buffer_name[30]; // Nombre del buffer en memoria compartida (dirección) 
    int waiting_time;
    int can_use_finisher = 0; // Variable para determinar si se puede usar el finalizador
    while(numer_Choice != 4){
/*        puts("/========================/");
        puts("          Menu            ");
        puts("/========================/");
        puts("1. Productor");
        puts("2. Consumidor");
        puts("3. Exit");*/
        printf(">>\n");
        scanf("%d", &numer_Choice);
        switch(numer_Choice){
        case 1:
            puts("Crear nuevo Productor");
            puts("Nombre del Buffer");
            scanf( "%s" , buffer_name );
            puts("Tiempo de espera");
            scanf("%d", &waiting_time);
/*            int res = new_producer(buffer_name, OBJECT_SIZE, waiting_time);
            if (res) { // Se creó el proceso exitósamente
               can_use_finisher = 1;
               puts("Productor creado exitósamente");
            }
            else {
               puts("Error en creación de proceso");             
            }*/
            break;
        case 2:
            puts("Crear nuevo Consumidor");
            puts("Nombre del Buffer");
            scanf( "%s" , buffer_name );
            puts("Tiempo de espera");
            scanf("%d", &waiting_time);
/*            int res = new_consumer(buffer_name, OBJECT_SIZE, waiting_time);
            if (res) { // Se creó el proceso exitósamente
               can_use_finisher = 1;
               puts("Consumidor creado exitósamente");
            }
            else {
               puts("Error en creación de proceso");             
            }*/
            break;
        case 3:
            if (can_use_finisher) {
               puts("Crear Finalizador");
               puts("Nombre del Buffer");
               scanf( "%s" , buffer_name );
/*               int res = new_finisher(buffer_name, OBJECT_SIZE);
               if (res) { // Se creó el proceso exitósamente
                  can_use_finisher = 0;
                  puts("Finalizador creado exitósamente");
               }
               else {
                  puts("Error en creación de proceso");             
               }*/
            } else {
              puts("No hay productores ni consumidores que finalizar");
            }
            break;
        default:
            puts("Exit");
            numer_Choice = 4;
            break;
        }

    }
    return 0;
}
