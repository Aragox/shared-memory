#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>
#include <sys/shm.h> 
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <time.h>

#include "circular_buffer.h"


int get_random(int upper, int lower) {
    int num = (rand() % ((upper) - (lower) + 1)) + (lower);
    return num; 
}

void execute_consumer(char *buffer_name, int average_time)
// Función que ejecuta el finisher para finalizar los productores, enviar mensajes de finalización a consumidores y libera el buffer
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
    circular_buffer *cb = (circular_buffer*)mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);  //Obtengo puntero al buffer en memoria compartida 
    if (cb == MAP_FAILED)
    {
        close(shm_fd);
        perror("\nError mmapping the file\n");
        exit(EXIT_FAILURE);
    }

//--------------------------------------------------------------------------------------------------------------------------
// EJECUTAR CICLO DE EJECUCIÓN
//--------------------------------------------------------------------------------------------------------------------------
    int delay = get_random(average_time*2, 1); // Setear tiempo promedio de espera;
    message* ptr = NULL;

    while (1) // Aumentar el número de consumidores activos
    {
      if (sem_trywait(get_sem_ptr(cb)) == 0) // El semáforo está disponible
      {
         increase_activeconsumers(cb); // El número de consumidores activos incrementa

         sem_post(get_sem_ptr(cb)); // Liberar semáforo

         break; // Fin del loop

      } else { // semáforo NO disponible
          sleep(delay); // Retraso en segundos

          if (delay < MAX_DELAY)
          {
             delay *= 2;
          }
      }
    } 

    delay = get_random(average_time*2, 1); // Resetear tiempo promedio de espera;   

    while (1) 
    {
      if (sem_trywait(get_sem_ptr(cb)) == 0) // El semáforo está disponible
      {

         if (get_count(cb) > 0) // Si hay mensajes que consumir
         {
            ptr = cb_dequeue(cb); // Consumir mensaje

            if (ptr != NULL) 
            {
               printf("\nShow consumed message...");
               printf("\n- Process ID: %d", (*ptr).pid);
               printf("\n- Is it the finalizer message?: %d", (*ptr).end_message);
               printf("\n- Date and time: %s", (*ptr).date_and_time);
               printf("\n- Key: %d", (*ptr).key);

               printf("\n#Messages in the buffer: %zu\n", get_count(cb));

               if ((*ptr).end_message == 1 || (*ptr).key == (getpid()%5)) // Consumidor finaliza
               {
                  decrease_activeconsumers(cb); // Disminuir consumidores activos

                  sem_post(get_sem_ptr(cb)); // Liberar semáforo

                  break; // Salir del loop
               }
            }
         } 

         delay = get_random(average_time*2, 1); // Resetear tiempo promedio de espera

         sem_post(get_sem_ptr(cb)); // Liberar semáforo

      } else { // semáforo NO disponible
          sleep(delay); // Retraso en segundos

          if (delay < MAX_DELAY)
          {
             delay *= 2;
          }
      }
    }
//--------------------------------------------------------------------------------------------------------------------------
// FIN DEL CICLO
//--------------------------------------------------------------------------------------------------------------------------
    // Desplegar ID del proceso y estadísticas de gestión (HACEEERRRR!!!!!)

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
  
      execute_consumer(argv[1], myatoi(argv[2]));      
    } 

    return 0;
}

