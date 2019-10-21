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

char random_nome[65][30] = {" The River ", " Heaven ", " Without You ", " The Middle ", " Senorita ", " Always ", " IDGAF ", " Singular ", " Nos ", " Love the Brain ", " Diamonds ", " FireWork ", " Honey ", " Smeels Like teen Spirit ", " Come As you Are ", " T.N.T ", " Back in Black ", " Highway To Hell ", " I miss You ", " Detesto Despedidas ", " Quem tem Joga ", " Dia D ", " Joane ", " Just Dance ", " Deixa ela beijar ", " 21 Guns ", " American Idioat ", " Shake it of ", " Perdoa ", " Burguesinha ", " Rosas ", " Coisa boa ", " Formation ", " Percect ", " Shape of You ", " Single Ladies ", " Yoyo ", " Boehemian Rhapsody ", " Wild Thoughts ", " Dias atras ", " Cheia de Manias ", " I love you 5 ", " I write sing not tragedies ", " Hard Times ", " Fake Happy ", " Mapa Mundi ", " Evoluiu ", " Bandito", " Do I Wanna Know ", " Terremoto ", " Calice ", " Poema ", " Curious ", " Believer ", " Lua Cheia ", " Taki Taki ", " Bad Liar ", " Problema Seu", " Pouca Pausa ", " We dont care " , " Partilhar " };
char random_autor[55][30] = {" Nirvana ", " ACDC ", " Imagine Dragons ", " 21 Pilots ", " Blink 182 ", " Pamore ", "Green Day ", " Queen ",  " Katy Perry ", " Ed Sheeran ", " Beyonce ", " Lady Gaga ", " Rihana ", " Dua Lipa" , " Lana Del Rey ", " Taylor Swift ", " Britney Spears ", " Justin T", " Lagum ", " Merlin ", " CPM22 ", " Raca Negra ", " Zeca Pagodinho ", " Alcione ", " Rita Lee ", " Cazuza ", " Anitta ", " Iza ", " Ludmila ", " Kevin o Cris ", " Lexa ", " Gloria Groove ", " Karol Conka",  " MC WM", " Jerry Smth " , " Mc Pocahontas ", " MC G15 ", " Mc Kekel ", " Mc Kevinho ", " Beethoven ", " Mozzart ", " AnaVitoria ", " Clarice Falcao ", " Silva ", " Cassia Eller ", " Los Hermanos ", " Vanguart ", " Ana Carolina ", " Rubel ", " Cicero ", " Tie ", " Caetano Veloso ", " O terno ", " Engenheiros do Hawaii ", " Seu Jorge "};
char random_genero[9][30] = {" rock ", " pop ", " funk ", " jazz ", " eletronica ", " classica ", " MPB ", " Samba ", " Pagode "};

int msgid;

int main() {
    int res;
    pthread_t a_thread[NUM_THREADS];
    void *thread_result;
    int thread_index;
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    while(1){
        for(thread_index = 0; thread_index < NUM_THREADS; thread_index++) {
            

            res = pthread_create(&(a_thread[thread_index]), NULL, create_song, (void *)thread_index);
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
    long tid;
    tid = (long)arg; 
    printf("thread %ld iniciou criacao\n\n", tid);
    struct song new_song;
        new_song.tipo_musica = 1;
        strncpy(new_song.nome, random_nome[(int)(rand() % (61)) + 0], 30);
        strncpy(new_song.autor, random_autor[(int)(rand() % (54)) + 0], 30);
        new_song.duracao = +(int)(9.0*rand()/(RAND_MAX+1.0));
        strncpy(new_song.genero, random_genero[((int)(rand() % (9)) + 0)], 30);

    // REGIÃO CRÍTICA!!
    if (msgsnd(msgid, (void *)&new_song, MAX_TEXT, 0) == -1) {
        fprintf(stderr, "falha ao adicionar a música na fila: %d \n", errno);
        exit(EXIT_FAILURE);
    } else {
        printf("criou a Música: \n Nome: %s\n Autor: %s \n Duracao: 00:0%i \n Gênero: %s\n",  new_song.nome, new_song.autor, new_song.duracao, new_song.genero);
        printf("thread %ld finalizou criacao\n\n", tid);
    }

    sleep(2);
    // FIM DA REGIÃO CRÍTICA
    pthread_exit(NULL);
}
