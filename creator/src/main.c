#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>
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

int main(int argc, char* argv[])
{
    char buffer_name[30]; // Nombre del buffer en memoria compartida (dirección) 

    printf("Program Name Is: %s",argv[0]); 
    if(argc==1) { 
       printf("\nNo Extra Command Line Argument Passed Other Than Program Name. Closing program..."); 
       return 0; 
    }
    if(argc>=2) 
    { 
        printf("\nNumber Of Arguments Passed: %d",argc); 
        printf("\n----Following Are The Command Line Arguments Passed----"); 
        for(int counter=0;counter<argc;counter++) {
            printf("\nargv[%d]: %s",counter,argv[counter]); 
        }

        int valid_arg2 = 1; // Indica si el argumento 2 es válido
        int valid_arg3 = 1; // Indica si el argumento 3 es válido
        size_t large;
        int cont = 1;
        int count2;
        char* pstring;
        while(cont < 3) { // Chequea que el segundo argumento sea string, y que el tercero sea un número
            large = strlen(argv[cont]);
            printf("\nLength %zu : %s",large,argv[cont]);
            count2 = 0;
            pstring = argv[cont];
            while(count2<large) {
                printf("\nChar %c",pstring[count2]);
                if (cont == 1) { // Verificar que los caracteres del segundo argumento sean letras
                   if (!isalpha(pstring[count2])) {
                      valid_arg2 = 0;
                      break;
                   }
                } else {
                   if (!isdigit(pstring[count2])) { // Verificar que los caracteres del tercer argumento sean números
                      valid_arg3 = 0;
                      break;
                   }
                }
                count2 = count2 + 1; 
            }       
            cont = cont + 1;
        }
  
        if (argc!=3) {
           printf("\nWrong number of arguments. Closing program..."); 
           return 0;
        }
        if (!valid_arg2) {
           printf("\nargv[2] is invalid (only letters are allowed). Closing program..."); 
           return 0;
        }
        if (!valid_arg3) {
           printf("\nargv[3] is invalid (only numbers are allowed). Closing program..."); 
           return 0;
        }

      printf("\n<<");
      printf("getpid(): %d", getpid()); // Obtener e imprimir el id del proceso
      printf(">>\n");        
    } 

    return 0;
}

