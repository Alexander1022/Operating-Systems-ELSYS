#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		return -1;
	}

	else
	{
		int file = 0;
		file = open(argv[1], O_RDONLY);
		char wow;
		size_t n;
		int counter = 0;
		int characters = 0;
		int i = 0;

		if(file < 0)
		{
			perror("Error");
		}
		else
		{	
			i = lseek(file, 0, SEEK_END);

			while(i != -1 && counter < 10)
			{

				read(file, &wow, 1);	

				i --;
				i = lseek(file, i, SEEK_SET);
			

				if(wow == '\n')
				{
					counter ++;
				}
					characters++;
			}

			if (counter > 9)
			{
    			lseek(file, 3, SEEK_CUR);
			}
			else
			{
				lseek(file, 0, SEEK_SET);
			}

			char buffer[characters];
		    read(file, buffer, characters);
		    write(1, buffer, characters);
		}
		close(file);
	}
}