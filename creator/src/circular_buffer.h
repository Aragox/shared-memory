#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

/*Implementación del circular_buffer tomada de 
https://stackoverflow.com/questions/827691/how-do-you-implement-a-circular-buffer-in-c*/

typedef struct circular_buffer
{
    char *buffer;     // data buffer
    char *buffer_end; // end of data buffer
    size_t capacity;  // maximum number of items in the buffer
    size_t count;     // number of items in the buffer
    size_t sz;        // size of each item in the buffer
    char *head;       // pointer to head
    char *tail;       // pointer to tail
} circular_buffer;


int cb_init(circular_buffer *cb, size_t capacity, size_t sz)
//Función que inicializa el buffer
{
    cb->buffer = malloc(capacity * sz);
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

