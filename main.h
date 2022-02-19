#define NUMERO_CONTENITORI 4
#define NUMERO_CONSUMATORI 200
#define QTA_MAX_CONTENITORE 50
#define handle_error(arg)   \
    {                       \
        perror(arg);        \
        exit(EXIT_FAILURE); \
    }

void consumatore_thread();
void inizializza_mutex();
void riempi_contenitore(int n_contenitore);