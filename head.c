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
    char *copy_buff = malloc(size);
    size_t offset = 0;
    size_t res;
    
    if((file = open("a.txt", O_RDONLY)) < 0)
    {
        write(1, "Invalid argument", 16);
    }

    else
    {
        file = open("a.txt", O_RDONLY);
        int j = 0;
        
        while((res = read(file, copy_buff + offset, 100000)) > 0) 
        {
                offset += res;
                if (offset + 100000 > size) 
                {
                        size *= 2;
                        copy_buff = realloc(copy_buff, size);
                }
        }
        
        //read(file, copy_buffer, 1999999999);
        
        int i = 0;
        int counter = 0;
        
        while(counter <10)
        {
		write(STDOUT_FILENO, (copy_buff + i), 1);
		i++;
		if(copy_buff[i] == '\n')
		{
			counter ++;
		}
        }
        
        copy_buff[size - 1] = '\0';
        write(STDOUT_FILENO, "\n", 1);
        
        close(file);
    }
    return 0;
}
