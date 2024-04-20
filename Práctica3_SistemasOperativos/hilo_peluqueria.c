#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stddef.h>
#include <semaphore.h>
pthread_t cliente;
sem_t aforo;
sem_t silla;
sem_t sofa;
sem_t dormir;
sem_t peluquero;
pthread_t clientes[50];
int indice[50];

void * funcion_cliente(void * args) {
    int thread_number = *((int *)args);
     sem_wait(&aforo); // Semáforo para que solamente puedan entrar 20 personas a la vez.
     printf("Se ha llenado la peluqueria con %d clientes \n", thread_number);
     sem_wait(&sofa); // Semáforo para que solamente se sienten 4 personas en el sofá a la vez.
     printf("Ya se han sentado %d clientes en el sofá \n", thread_number);
     sem_wait(&silla); // Semáforo para que solamente se siente un cliente en la silla.
     printf("Ya se ha sentado el cliente %d de la peluqueria \n", thread_number);
     sem_wait(&peluquero);
     printf("El único peluquero ya ha terminado de cortar el pelo al cliente \n");
     sem_post(&silla);
     sem_post(&peluquero);
     sem_post(&sofa);
     sem_post(&aforo);
     pthread_exit(NULL);
}

void * funcion_peluquero(void * args) {
    while(1) {
       sem_wait(&peluquero); // Espera a qué esté listo un cliente.
       sleep(5); // Corta el pelo durante esté intervalo de 5 segundos.
       fflush(stdout);
       sleep(1);
       sem_post(&peluquero);
    }

}

void main(void) {
    sem_init(&aforo, 0, 20);
    sem_init(&peluquero, 0, 1);
    sem_init(&silla, 0, 1);
    sem_init(&sofa, 0, 4);
    sem_init(&dormir, 0, 5);

    for(int i = 1; i <= 50; i++) {
        indice[i] = i;
        pthread_create(&clientes[i], NULL, funcion_cliente, &indice[i]);
        sleep(1);
    }

    for(int i = 1; i <= 50; i++) {
            pthread_join(clientes[i], NULL);
        }


        sem_destroy(&aforo);
        sem_destroy(&peluquero);
        sem_destroy(&silla);
        sem_destroy(&sofa);
}
