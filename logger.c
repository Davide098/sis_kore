#include "main.h"
#include <fcntl.h>

int main()
{
    int fd_pipe, fd_output;

    if ((fd_pipe = open(NOME_PIPE, O_RDONLY)) == -1)
    {
        handle_error("open fifo")
    }

    if ((fd_output = open("./log.txt", O_WRONLY | O_CREAT | O_APPEND , 0777)) == -1)
    {
        handle_error("open output");
    }

    while (1)
    {

        char buffer[BUFFER_SIZE];
        memset(buffer, '\0', sizeof(char) * BUFFER_SIZE);
        if (read(fd_pipe, &buffer, sizeof(char) * BUFFER_SIZE) == -1)
        {
            handle_error("read")
        }
        printf("RICEVUTO\n");
        int c = 0;
        while (buffer[c] != '\0')
        {
            ++c;
        }

        write(fd_output, buffer, sizeof(char) * c);

        if (buffer[0] == '\0')
        {
            printf("ricevuto EOF\n");
            break;
        }
    }
}