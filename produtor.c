#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

#define NUM_THREADS 6
#define MAX_TEXT 512

#include "song.h"

void *create_song(void *arg);

char random_phrases[15][50] = {" rock ", " pop ", " funk ", " macacos ", " sabrina ", " super ", " super ", " amor ", " destruidores ", " the ", " the ", " detonator ", "love", "importa", "UAU"};

int msgid;

int main() {
    int res;
    pthread_t a_thread[NUM_THREADS];
    void *thread_result;
    int thread_index;
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    while(1){
        for(thread_index = 0; thread_index < NUM_THREADS; thread_index++) {

            res = pthread_create(&(a_thread[thread_index]), NULL, create_song, (void *)&thread_index);
            if (res != 0) {
                perror("Criacao de Thread falhou");
                exit(EXIT_FAILURE);
            }
            sleep(1);
        }

        for(thread_index = NUM_THREADS - 1; thread_index >= 0; thread_index--) {
            res = pthread_join(a_thread[thread_index], &thread_result);
            if(res != 0){
                perror("Erro no join de thread");
                exit(EXIT_FAILURE);
            }
        }
    }
    printf("Processo acabou\n");
    exit(EXIT_SUCCESS);
}

void *create_song(void *arg) {
    struct song new_song = {
        1,
        *random_phrases[(int)(rand() % (14 - 0 + 1)) + 0],
        *random_phrases[(int)(rand() % (14 - 0 + 1)) + 0],
        1+(int)(9.0*rand()/(RAND_MAX+1.0)),
        *random_phrases[((int)(rand() % (14 - 0 + 1)) + 0)]
    };
    // REGIÃO CRÍTICA!!
    if (msgsnd(msgid, (void *)&new_song, MAX_TEXT, 0) == -1) {
        fprintf(stderr, "falha ao adicionar a música na fila: %d \n", errno);
        exit(EXIT_FAILURE);
    } else {
        printf("criou a música: %s\n", new_song);
    }

    sleep(1);
    // FIM DA REGIÃO CRÍTICA
    pthread_exit(NULL);
}