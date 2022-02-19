#include "main.h"
#include <fcntl.h>

int main()
{
    int fd_pipe, fd_output;

    if ((fd_pipe = open(NOME_PIPE, O_RDONLY)) == -1)
    {
        handle_error("open fifo")
    }

    while (1)
    {

        char buffer[256];
        memset(buffer, '\0', sizeof(char)*256);
        if (read(fd_pipe, &buffer, sizeof(char) * 256) == -1)
        {
            handle_error("read")
        }
        for (int i = 0; i < 256; i++)
        {
            printf("%c", buffer[i]);
        }

        if (buffer[0] == '\0')
        {
            printf("ricevuto EOF\n");
            break;
        }
    }
}