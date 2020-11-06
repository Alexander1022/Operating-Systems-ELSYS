#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int file = 0;
    int res = 0;
    int offset = 0;
    size_t size = 100000;
    char *copy_buff = malloc(size);

    file = open("a.txt", O_RDONLY);

    if(file < 0)
    {
        perror("Error");
    }

    else
    {
        int j = 0;
        int i = 0;
        int counter = 0;
        while((res = read(file, copy_buff + offset, 100000)) > 0 || counter < 10)
        {
            write(STDOUT_FILENO, (copy_buff + i), 1);
            i++;
            if(copy_buff[i] == '\n')
            {
              counter ++;
            }
            offset = offset + res;
            if(offset + 100000 > size)
            {
              size = size * 2;
              copy_buff = realloc(copy_buff, size);
            }
        }

        copy_buff[size - 1] = '\0';
        write(STDOUT_FILENO, "\n", 1);

        close(file);
    }
    return 0;
}
