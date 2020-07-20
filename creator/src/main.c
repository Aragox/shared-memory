#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>
#include <sys/shm.h> 
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>

#include "circular_buffer.h"


void initialize_sharedmemory_variables(char *buffer_name, int capacity)
// Función que crea e inicializa el buffer en memoria compartida. Todas las variables y banderas del enunciado son también parte del buffer
{
    //File descriptor de la memoria compartida
    int shm_fd;

    // Crear el objeto de memoria compartida 
    shm_fd = shm_open(buffer_name, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1)
    {
        perror("\nError opening file for writing\n");
        exit(EXIT_FAILURE);
    }     

    // Configurar el tamaño del objeto en memoria compartida
    ftruncate(shm_fd, BUFFER_SIZE);

    // Mapear memoria del objeto compartido en memoria
    circular_buffer *cb = (circular_buffer*)mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);  //Obtengo puntero al buffer en memoria compartida 

    if (cb == MAP_FAILED)
    {
        close(shm_fd);
        perror("\nError mmapping the file\n");
        exit(EXIT_FAILURE);
    }

    // Inicializar la memoria compartida 
      cb_init(cb); 
      printf("\nBuffer circular inicializado correctamente en memoria compartida\n");

//--------------------------------------------------------------------------------------------------------------------------
//##########################################################################################################################
// PRUEBAS AL BUFFER CIRCULAR EN MEMORIA COMPARTIDA. ESTO ES CÓDIGO INNECESARIO EN EL PROGRAMA CREADOR
//##########################################################################################################################
//--------------------------------------------------------------------------------------------------------------------------
      char a[] = "Hello world";
      char b[] = "Yeah";
      char c[] = "Cyanide and happiness";
      char d[] = "One more!";
      
      message msg1;
      message msg2;
      message msg3;
      message msg4; 

      message* a1 = &msg1;
      message* b1 = &msg2;
      message* c1 = &msg3;
      message* d1 = &msg4;

      (*a1).pid = 0;
      (*a1).end_message = 0;
      (*a1).key = 0;
      (*b1).pid = 0;
      (*b1).end_message = 0;
      (*b1).key = 0;
      (*c1).pid = 0;
      (*c1).end_message = 0;
      (*c1).key = 0;
      (*d1).pid = 0;
      (*d1).end_message = 0;
      (*d1).key = 0;
      memcpy ((*a1).date_and_time, a, sizeof(a)); // Copiar string 
      memcpy ((*b1).date_and_time, b, sizeof(b)); // Copiar string
      memcpy ((*c1).date_and_time, c, sizeof(c)); // Copiar string
      memcpy ((*d1).date_and_time, d, sizeof(d)); // Copiar string

      cb_enqueue(cb, a1);
      cb_enqueue(cb, b1);
      cb_enqueue(cb, c1);
      cb_enqueue(cb, d1); // Intento agregar otro item más aunque el buffer ya está lleno (la idea es que se maneje el error)

      printf("\n#Mensajes en el buffer: %zu\n", cb->count);
 
//--------------------------------------------------------------------------------------------------------------------------
//##########################################################################################################################
// FIN DE LAS PRUEBAS
//##########################################################################################################################
//--------------------------------------------------------------------------------------------------------------------------  
    // Liberar la memoria mapeada (liberar el buffer)
    if (munmap(cb, BUFFER_SIZE) == -1)
    {
        close(shm_fd);
        perror("\nError un-mmapping the file\n");
        exit(EXIT_FAILURE);
    }

    // Cerrar File.
    close(shm_fd);
}   
 
int myatoi(char* str) 
/* Función atoi() implementada en C. Hace casting de una hilera de chars a un sólo número y lo retorna. Obtenida de
https://www.geeksforgeeks.org/write-your-own-atoi/*/
{ 
    // Initialize result 
    int res = 0; 
  
    // Iterate through all characters 
    // of input string and update result 
    for (int i = 0; str[i] != '\0'; ++i) { 
        res = res * 10 + str[i] - '0'; 
    }
    // return result. 
    return res; 
} 

int main(int argc, char* argv[])
{
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
           printf("\nWrong number of arguments (only 3 required). Closing program..."); 
           return 0;
        }
        if (!valid_arg2) {
           printf("\nargv[1] is invalid (only letters are allowed). Closing program..."); 
           return 0;
        }
        if (!valid_arg3) {
           printf("\nargv[2] is invalid (only numbers are allowed). Closing program..."); 
           return 0;
        }

      printf("\n<<");
      printf("Process ID: %d", getpid()); // Obtener e imprimir el id del proceso
      printf(">>\n");
  
      initialize_sharedmemory_variables(argv[1], myatoi(argv[2]));      
    } 

    return 0;
}

