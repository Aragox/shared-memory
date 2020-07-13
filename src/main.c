#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main()
{
    Node* queue_Producers;
    Node* queue_Consumers;
    Node* buffer;
   // bool wait;
    //bool signal;
    int Snumber_of_processes; //
    int numer_Choice = 0;
    char process_name_producer[50];
    char process_name_consumer[50];
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
            puts("Nombre del Proceso");
            scanf( "%s" , process_name_producer );
            puts("Tiempo de espera");
            scanf("%d", &waiting_time);
            break;
        case 2:
            puts("Agregar nuevo Consumidor");
            puts("Nombre del Proceso");
            scanf( "%s" , process_name_consumer );
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
