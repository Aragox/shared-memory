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


int get_random(int upper, int lower)
// Función que obtiene número aleatorio en el rango upper-lower inclusivo
{
    int num = (rand() % ((upper) - (lower) + 1)) + (lower);
    return num; 
}

int exponential_backoff(int delay)
// Función que aumenta el tiempo a esperar (delay) exponencialmente. Retorna el delay actualizado
{
     if (delay < MAX_DELAY)
     {
        delay = get_random(delay*2, 1); // Obtener tiempo promedio de espera, aleatoriamente; 
     }
     printf("\nSlept %d seconds", delay);
     sleep(delay); // Retraso en segundos

     return delay;
}

void execute_finisher(char *buffer_name, int average_time)
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
    time_t curtime; // Tiempo actual 
    int delay = average_time; // Setear tiempo promedio de espera;
    int sem_time;
    int process_time;
    int initial_time;
    int final_time;
    int number_of_consumers;
    int number_of_producers;
    while (1) // Tratar de setear bandera de finalización para los productores
    {
      if (sem_trywait(get_sem_ptr(cb)) == 0) // El semáforo está disponible
      {
         change_endsignal(cb); // Setear bandera de finalización para los productores

         sem_post(get_sem_ptr(cb)); // Liberar semáforo

         break; // Fin del loop

      } else { // semáforo NO disponible
        initial_time = time(&curtime);
        exponential_backoff(delay);
        final_time = time(&curtime);
        sem_time = sem_time + (final_time-initial_time); 
      }
    } 
	
    delay = average_time; // Resetear tiempo promedio de espera;   
    number_of_producers = get_activeproducers(cb);
    number_of_consumers = get_activeconsumers(cb);
    while (1) 
    {
      if (sem_trywait(get_sem_ptr(cb)) == 0) // El semáforo está disponible
      {

         if (get_activeproducers(cb) > 0 || get_activeconsumers(cb) > 0) // Si hay procesos activos
         {

            if (get_count(cb) < BUFFER_CAPACITY) { // Si Buffer no está lleno
               time(&curtime); 

               char date_and_time[DATE_AND_TIME_LENGTH];
               memcpy(date_and_time, ctime(&curtime), sizeof(date_and_time)); // Inicializar y crear string de fecha y hora

               message msg;
               message* special_msg = &msg;
               (*special_msg).pid = getpid();
               (*special_msg).end_message = 1;
               (*special_msg).key = get_random(4, 0);
               memcpy ((*special_msg).date_and_time, date_and_time, sizeof(date_and_time)); // Copiar string de fecha y hora

               printf("\nShow produced message...");
               printf("\n- Process ID: %d", (*special_msg).pid);
               printf("\n- Is it the finalizer message?: %d", (*special_msg).end_message);
               printf("\n- Key: %d", (*special_msg).key);
               printf("\n- Date and time: %s", (*special_msg).date_and_time);

               printf("\n#Messages in the buffer: %zu\n", get_count(cb));

               cb_enqueue(cb, special_msg); // Push de mensaje especial 

               delay = average_time; // Resetear tiempo promedio de espera 

               sem_post(get_sem_ptr(cb)); // Liberar semáforo
            } else { // El Buffer está lleno
              sem_post(get_sem_ptr(cb)); // Liberar semáforo

	      initial_time = time(&curtime);
	      delay = exponential_backoff(delay);
	      final_time = time(&curtime);
	      process_time = process_time + (final_time-initial_time); 
              
            }
         } else { // Ya finalizaron todos los procesos
             sem_post(get_sem_ptr(cb)); // Liberar semáforo

             break; // Salir del ciclo
         }
      } else { // semáforo NO disponible
         initial_time = time(&curtime);
	 delay = exponential_backoff(delay);
	 final_time = time(&curtime);
	 process_time = process_time + (final_time-initial_time); 
      }
    }
//--------------------------------------------------------------------------------------------------------------------------
// FIN DEL CICLO
//--------------------------------------------------------------------------------------------------------------------------
    printf("\n End finisher process");
    printf("\n Estadistics");
    printf("\n Semaphore Time = %d seconds", sem_time);
    printf("\n Process Time = %d seconds", process_time);
    printf("\n Active Processes = %d", number_of_producers);
    printf("\n Active Consumers = %d", number_of_consumers);
    printf("\n Number of items in the Buffer = %ld", get_count(cb));
    // Liberar la memoria mapeada (liberar el buffer)
    if (munmap(cb, BUFFER_SIZE) == -1)
    {
        close(shm_fd);
        perror("\nError un-mmapping the file\n");
        exit(EXIT_FAILURE);
    }

    /* Destruír memoria compartida */
    shm_unlink(buffer_name);

    // Cerrar File.
    close(shm_fd);

    printf("\nShared Buffer Memory Released!\n");

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
    if (argc!=3) {
       printf("\nWrong number of arguments (3 required). Closing program...\n"); 
       return 0;
    }
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
  
        if (!valid_arg2) {
           printf("\nargv[1] is invalid (only letters are allowed). Closing program...\n"); 
           return 0;
        }
        if (!valid_arg3) {
           printf("\nargv[2] is invalid (only numbers are allowed). Closing program...\n"); 
           return 0;
        }

      printf("\n<<");
      printf("Process ID: %d", getpid()); // Obtener e imprimir el id del proceso
      printf(">>\n");
     
     execute_finisher(argv[1], myatoi(argv[2]));       

    return 0;
}

