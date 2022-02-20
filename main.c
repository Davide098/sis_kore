
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>
#include "main.h"

int contenitori[NUMERO_CONTENITORI];
pthread_mutex_t mutexes[NUMERO_CONTENITORI]; // mutex dei contenitori
int fd_pipe;

int main()
{
    inizializza_mutex();

    pthread_t consumatori[NUMERO_CONSUMATORI];
    if ((fd_pipe = open(NOME_PIPE, O_WRONLY)) == -1)
    {
        handle_error("open");
        exit(EXIT_FAILURE);
    }
    for (long int i = 0; i < NUMERO_CONSUMATORI; i++)
    {
        if (pthread_create(&consumatori[i], NULL, (void *)consumatore_thread, (long *)i) != 0)
        {
            handle_error("pthread_create")
        }
    }

    /*join dei thread*/
    for (long int i = 0; i < NUMERO_CONSUMATORI; i++)
    {
        if (pthread_join(consumatori[i], NULL) != 0)
        {
            handle_error("pthread_join");
        }
    }
    for (int i = 0; i < NUMERO_CONTENITORI; i++)
    {
        printf("Contenitore[%d] = %d\n", i, contenitori[i]);
    }

    char messaggio[BUFFER_SIZE];
    memset(messaggio, '\0', sizeof(char) * BUFFER_SIZE);
    scrivi_su_pipe(messaggio);

    return 0;
}

int qta_alcol_per_cocktail = 5;
int qta_bevanda_per_cocktail = 5;

void consumatore_thread(long *id)
{
    printf("THREAD INIZIA %ld\n", (long)id);

    // contenitore alcol tra 0 e 1
    int alcol = rand() % 2;
    // contenitore bevanda tra 2 e 3
    int bevanda = 2 + (rand() % 2);

    if (pthread_mutex_lock(&mutexes[alcol]) != 0)
    {
        printf("error alcol");
    }

    if (contenitori[alcol] < qta_alcol_per_cocktail)
    {
        riempi_contenitore(alcol);
        printf("Thread %ld ha fatto riempire \n\tALCOL: %d\n", (long)id, alcol);
    }
    contenitori[alcol] -= qta_alcol_per_cocktail;
    pthread_mutex_unlock(&mutexes[alcol]);

    if (pthread_mutex_lock(&mutexes[bevanda]) != 0)
    {
        printf("error bevanda");
    }

    if (contenitori[bevanda] < qta_bevanda_per_cocktail)
    {
        riempi_contenitore(bevanda);
        printf("Thread %ld ha fatto riempire \n\tBEVANDA: %d\n", (long)id, bevanda);
    }

    contenitori[bevanda] -= qta_bevanda_per_cocktail;
    pthread_mutex_unlock(&mutexes[bevanda]);
    printf("Thread %ld ha preparato\n\tALCOL: %d\n\tBEVANDA: %d\n", (long)id, alcol, bevanda);
    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', sizeof(char) * BUFFER_SIZE);
    char *msg = crea_messaggio(alcol, bevanda, (long)id);
    memcpy(buffer, msg, sizeof(char) * (strlen(msg) + 1));

    scrivi_su_pipe(buffer);
}

void riempi_contenitore(int n_contenitore)
{
    contenitori[n_contenitore] = QTA_MAX_CONTENITORE;
}

void inizializza_mutex()
{
    for (int i = 0; i < NUMERO_CONTENITORI; i++)
    {
        if (pthread_mutex_init(&mutexes[i], NULL) == -1)
            handle_error("pthread_mutex_init");
    }
}

void scrivi_su_pipe(char messaggio[BUFFER_SIZE])
{
    if (write(fd_pipe, messaggio, sizeof(char) * BUFFER_SIZE) == -1)
    {
        handle_error("write");
    }
}

char *crea_messaggio(int alcol, int bevanda, long int id)
{

    char *a1 = "Vodka\n";
    char *a2 = "Gin\n";
    char *b1 = "Cola\n";
    char *b2 = "Tonic\n";
    char *nome_alcol, *nome_bevanda;
    char *nome_alcol_esteso = "\tALCOL: ";
    char *nome_bevanda_esteso = "\tBEAVANDA: ";
    char *nome_thread;
    char *nome_thread_esteso = "THREAD: ";

    nome_thread = malloc(sizeof(char) * (strlen(nome_thread_esteso) + 1));
    sprintf(nome_thread, "%s%ld\n", nome_thread_esteso, id);
    switch (alcol)
    {
    case 0:
        nome_alcol = malloc(sizeof(char) * (strlen(a1) + strlen(nome_alcol_esteso) + 1));
        sprintf(nome_alcol, "%s%s", nome_alcol_esteso, a1);
        break;
    case 1:
        nome_alcol = malloc(sizeof(char) * (strlen(a2) + strlen(nome_alcol_esteso) + 1));
        sprintf(nome_alcol, "%s%s", nome_alcol_esteso, a2);
        break;

    default:
        break;
    }

    switch (bevanda)
    {
    case 2:
        nome_bevanda = malloc(sizeof(char) * (strlen(b1) + strlen(nome_bevanda_esteso) + 1));
        sprintf(nome_bevanda, "%s%s", nome_bevanda_esteso, b1);
        break;
    case 3:
        nome_bevanda = malloc(sizeof(char) * (strlen(b2) + strlen(nome_bevanda_esteso) + 1));
        sprintf(nome_bevanda, "%s%s", nome_bevanda_esteso, b2);
        break;

    default:
        break;
    }

    char *messaggio = malloc(sizeof(char) * (strlen(nome_bevanda) + strlen(nome_alcol) + strlen(nome_thread) + 1));
    sprintf(messaggio, "%s%s%s", nome_thread, nome_alcol, nome_bevanda);
    return messaggio;
}