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
    size_t size = 4096;
    char buffer[size] = {0};
    int n;

    file = open("a.txt", O_RDONLY);

    if(file < 0)
    {
        perror("Error");
    }

    else
    {
        int i =0;
        int j = 0;
        int counter = 0;
        int counter_2 = 0;

        while((n=read(file, buffer, 4096)) > 0)
       	{
       		while(buffer[i] != '\0')
          {
            if(buffer[i] == '\n')
            {
              counter ++;
            }
            i++;
          }
          if(counter == 9)
          {
            while(buffer[j] != 0 || counter != 0)
            {
              write(1, (buffer + i), 1);
              if(buffer[i] == '\n')
              {
                counter --;
              }
              i++;
            }
          }
          else if(counter >-10)
          {
            while(counter_2 < 10)
            {
              write(1, buffer[i], 1);
              if(buffer[i] == '\n')
              {
                counter_2 ++;
              }
              i++;
            }
          }
        }
        write(1, "Hello\n", 5);
        buffer[size - 1] = '\0';
        write(STDOUT_FILENO, "\n", 1);
        close(file);
    }
    return 0;
}
