#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

/*Parte de la implementación del circular_buffer obtenida de 
https://stackoverflow.com/questions/827691/how-do-you-implement-a-circular-buffer-in-c*/

typedef struct circular_buffer
{
    char *buffer;     // datos del buffer
    char *buffer_end; // puntero al final de los datos del buffer
    size_t capacity;  // máximo número de items en el buffer
    size_t count;     // número de items en el buffer
    size_t sz;        // tamaño de cada item en el buffer
    char *head;       // puntero a head
    char *tail;       // puntero a tail

    int end_signal; // Bandera de finalización para los productores (1 ó -1)
    int active_producers; //Productores activos
    int active_consumers; //Consumidores activos

    sem_t sem1; // Semáforo
} circular_buffer;


int cb_init(circular_buffer *cb, size_t capacity, size_t sz)
//Función que inicializa el buffer
{
    cb->buffer = malloc(capacity * sz); // Inicializar buffer
    if(cb->buffer == NULL) {
        printf("\nFailed to allocate buffer memory dynamically");
        return 1;
    }
    cb->buffer_end = (char *)cb->buffer + capacity * sz;
    cb->capacity = capacity;
    cb->count = 0;
    cb->sz = sz;
    cb->head = cb->buffer;
    cb->tail = cb->buffer;

    cb->end_signal = 0; // Inicializar variables y banderas
    cb->active_producers = 0; 
    cb->active_consumers = 0; 

    sem_init(&cb->sem1, 1, 1); // Inicializar semáforo
    return 0;
}

void cb_free(circular_buffer *cb)
//Función que libera el buffer
{
    free(cb->buffer);
}

void cb_push_back(circular_buffer *cb, const void *item)
//Función que agrega un item en head
{
    if(cb->count == cb->capacity){
        printf("\nBuffer is full!");
        return;
    }
    memcpy(cb->head, item, cb->sz + 1);
    cb->head = (char*)cb->head + cb->sz;
    if(cb->head == cb->buffer_end)
        cb->head = cb->buffer;
    cb->count++;
}

char* cb_pop_front(circular_buffer *cb)
//Función que elimina un item en tail
{
    if(cb->count == 0){
        printf("\nBuffer is empty!");
        return NULL;
    }
    char* temp = cb->tail;
    cb->tail = (char*)cb->tail + cb->sz;
    if(cb->tail == cb->buffer_end)
        cb->tail = cb->buffer;
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

size_t get_capacity(circular_buffer *cb) {
    return cb->capacity;
}

