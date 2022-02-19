#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#define NUMERO_CONTENITORI 4
#define NUMERO_CONSUMATORI 5
#define QTA_MAX_CONTENITORE 50
#define NOME_PIPE "/tmp/my_pipe"
#define handle_error(arg)   \
    {                       \
        perror(arg);        \
        exit(EXIT_FAILURE); \
    }
void consumatore_thread();
void inizializza_mutex();
void riempi_contenitore(int n_contenitore);