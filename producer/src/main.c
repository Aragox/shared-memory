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

char str_to_print[] = "Hello world";
void execute_producer(char *buffer_name, int capacity, message *messageP)
// Función que ejecuta el finisher para finalizar los productores, enviar mensajes de finalización a consumidores y liberar el buffer
{
    //File descriptor de la memoria compartida
    int shm_fd;

    // Crear el objeto de memoria compartida 
    shm_fd = shm_open(buffer_name, O_RDWR, 0666);

    if (shm_fd == -1)
    {
        perror("\nError opening file for writing\n");
        exit(EXIT_FAILURE);
    }     

    // Configurar el tamaño del objeto en memoria compartida
    ftruncate(shm_fd, BUFFER_SIZE);

    // Mapear memoria del objeto compartido en memoria
    circular_buffer *cb = (circular_buffer*)mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);  //Obtengo puntero al 	   buffer en memoria compartida  
    

    if (cb == MAP_FAILED)
    {
        close(shm_fd);
        perror("\nError mmapping the file\n");
        exit(EXIT_FAILURE);
    }
      
    
    
      
     // memcpy ((*a1).date_and_time, a, sizeof(a)); // Copiar string 
      //char a[] = "Hello world";
      memcpy ((*messageP).date_and_time, str_to_print, sizeof(str_to_print));
      cb_enqueue(cb, messageP);
   
      // Incrementar la cantidad de productores  
      increase_activeproducers(cb);
      printf("\n#Mensajes en el buffer: %zu\n", cb->count);
/*
//--------------------------------------------------------------------------------------------------------------------------
//##########################################################################################################################
// PRUEBAS AL BUFFER CIRCULAR EN MEMORIA COMPARTIDA. ESTO ES CÓDIGO INNECESARIO EN EL PROGRAMA CREADOR
//##########################################################################################################################
//--------------------------------------------------------------------------------------------------------------------------
      char a[] = "Hello world";
      char b[] = "Yeah";
      char c[] = "Cyanide and happiness";
      char d[] = "One more!";
      char *ptr = NULL;
      cb_push_back(cb, a);
      cb_push_back(cb, b);
      cb_push_back(cb, c);
      cb_push_back(cb, d); // Intento agregar otro item más aunque el buffer ya está lleno (la idea es que se maneje el error)
      ptr = cb_pop_front(cb);
      printf("\nValor: %s", ptr);
      ptr = cb_pop_front(cb);
      printf("\nValor: %s", ptr);
      ptr = cb_pop_front(cb);
      printf("\nValor: %s", ptr);      
      ptr = cb_pop_front(cb);
      printf("\nValor: %s", ptr); // Intento sacar otro item más aunque el buffer ya está vacío (la idea es que se maneje el error)
      printf("\ncount: %zu", cb->count);
      cb_free(cb);  
//--------------------------------------------------------------------------------------------------------------------------
//##########################################################################################################################
// FIN DE LAS PRUEBAS
//##########################################################################################################################
//--------------------------------------------------------------------------------------------------------------------------  
*/
    // Liberar la memoria mapeada (liberar el buffer)
   if (munmap(cb, BUFFER_SIZE) == -1)
    {
        close(shm_fd);
        perror("\nError un-mmapping the file\n");
        exit(EXIT_FAILURE);
    }
		
    /* Destruír memoria compartida */
   // shm_unlink(buffer_name);

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
message* create_new_message(int pidP, int end_messageP, int keyP){
      

      message msg1;


      message* message_producer = &msg1;


      (*message_producer).pid = pidP;
      (*message_producer).end_message = end_messageP;
      (*message_producer).key = keyP;
      
      return message_producer;

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
     
      message* message = create_new_message(0,0,0);
      execute_producer(argv[1], myatoi(argv[2]),message);      
    } 

    return 0;
}
