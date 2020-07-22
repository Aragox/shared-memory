#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define BUFFER_CAPACITY (3) // máximo número de items en el buffer
#define DATE_AND_TIME_LENGTH (128) // largo del string de la fecha y hora
#define MAX_DELAY (8) // Máximo tiempo en segundos que un proceso llega a esperar

typedef struct message  // Struct del buffer junto con todas las variables, banderas y semáforos
{
    int pid; //Id del proceso productor o finalizador
    int end_message; //Indica si el mensaje fue producido por el finalizador (0 ó 1)
    char date_and_time[DATE_AND_TIME_LENGTH]; //Un string que contiene el tiempo y fecha actuales 
    int key; //Llave aleatoria entre 0-4     
} message;

/*Parte de la implementación del circular_buffer obtenida de 
https://stackoverflow.com/questions/827691/how-do-you-implement-a-circular-buffer-in-c*/

typedef struct circular_buffer // Struct del buffer circular junto con todas las variables, banderas y semáforos
{
    message buffer[BUFFER_CAPACITY]; // datos del buffer     
    size_t count;     // número de items en el buffer
    int front;       // posición
    int rear;       // posición

    int end_signal; // Bandera de finalización para los productores (1 ó -1)
    int active_producers; //Productores activos
    int active_consumers; //Consumidores activos

    sem_t sem1; // Semáforo

} circular_buffer;


#define BUFFER_SIZE (sizeof(circular_buffer)) // Definir el tamaño del struct


void cb_init(circular_buffer *cb)
//Función que inicializa el buffer
{
    for (int i = 0; i < BUFFER_CAPACITY; i++) { // Inicializar buffer
        memset(cb->buffer[i].date_and_time, '\0', sizeof(DATE_AND_TIME_LENGTH));
    } 

    cb->count = 0;
    cb->front = -1;
    cb->rear = -1;

    cb->end_signal = 1; // Inicializar variables y banderas
    cb->active_producers = 0; 
    cb->active_consumers = 0; 

    sem_init(&cb->sem1, 1, 1); // Inicializar semáforo

}

void cb_enqueue(circular_buffer *cb, message *item)
//Función que agrega un item en head
{
    if((cb->front == 0 && cb->rear == BUFFER_CAPACITY-1) || (cb->rear == (cb->front-1)%(BUFFER_CAPACITY-1))){
        printf("\nBuffer is full!");
        return;
    }  
    else if (cb->front == -1) //Insertar primer elemento
    { 
        cb->front = cb->rear = 0; 
    } 
  
    else if (cb->rear == BUFFER_CAPACITY-1 && cb->front != 0) 
    { 
        cb->rear = 0; 
    } 
  
    else
    { 
        cb->rear = (cb->rear + 1)%(BUFFER_CAPACITY); 
    } 
    cb->buffer[cb->rear].pid = (*item).pid; // Copiar variables del mensaje
    cb->buffer[cb->rear].end_message = (*item).end_message;
    cb->buffer[cb->rear].key = (*item).key;
    memcpy (cb->buffer[cb->rear].date_and_time, (*item).date_and_time, sizeof((*item).date_and_time)); // Copiar string de fecha y hora

    cb->count++;
}

message* cb_dequeue(circular_buffer *cb)
//Función que elimina un item en tail
{
    if (cb->front == -1) 
    { 
        printf("\nBuffer is empty!");
        return NULL; 
    } 
    message* temp = &cb->buffer[cb->front]; 

    if (cb->front == cb->rear) 
    { 
        cb->front = -1; 
        cb->rear = -1; 
    } 
    else if (cb->front == BUFFER_CAPACITY-1) 
    { 
        cb->front = 0; 
    }
    else
    {
        cb->front = (cb->front + 1)%(BUFFER_CAPACITY); 
    }
    cb->count--;
  
    return temp; 
}

void increase_activeproducers(circular_buffer *cb) {
     cb->active_producers++; 
}

void decrease_activeproducers(circular_buffer *cb) {
     if (cb->active_producers > 0) {
        cb->active_producers--;
     } 
}

void increase_activeconsumers(circular_buffer *cb) {
     cb->active_consumers++; 
}

void decrease_activeconsumers(circular_buffer *cb) {
     if (cb->active_consumers > 0) {
        cb->active_consumers--;
     } 
}

void change_endsignal(circular_buffer *cb) {
    cb->end_signal = (cb->end_signal * -1);
}

int get_endsignal(circular_buffer *cb) {
    return cb->end_signal;
}

int get_activeproducers(circular_buffer *cb) {
    return cb->active_producers;
}

int get_activeconsumers(circular_buffer *cb) {
    return cb->active_consumers;
}

size_t get_count(circular_buffer *cb) {
    return cb->count;
}

sem_t* get_sem_ptr(circular_buffer *cb) {
    return &cb->sem1;
}

int get_rear(circular_buffer *cb) {
    return cb->rear; 
} 

int get_front(circular_buffer *cb) {
    return cb->front; 
} 
