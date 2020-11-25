#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		write(1, "You have to write something here!", 33);
	} 

	else if(argc > 2)
	{
		for(int f = 1 ; f < argc ; f++)
		{
			int name_count = strlen(argv[f]);
			write(1, "==> ", 4);
			write(1, argv[f], name_count);
			write(1, " <==", 4);
			write(1, "\n", 1);
			int file = 0;
			file = open(argv[f], O_RDONLY);
			char wow;
			size_t n;
			int counter = 0;
			int characters = 0;
			int i = 0;

			if(file < 0)
			{
				return -1;
			}

			else
			{	
				i = lseek(file, 0, SEEK_END);

				while(i != -1 && counter != 10)
				{

					read(file, &wow, 1);	
					i --;
					lseek(file, i, SEEK_SET);
				

					if(wow == '\n')
					{
						counter ++;
					}

					characters++;
				}

				if (counter >= 10)
				{
					characters = characters - 2;
					char buffer[characters];
	    			lseek(file, 2, SEEK_CUR);
			    	read(file, buffer, characters);
			    	write(1, buffer, characters);
				}

				else
				{
					characters = characters - 1;
					char buffer[characters];
					lseek(file, 0, SEEK_SET);
				    read(file, buffer, characters);
				    write(1, buffer, characters);
				}
				close(file);
				if(f != argc - 1)
				{
					write(1, "\n", 1);
				}
			}
		}
	}

	else if(argc == 2)
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
			return -1;
		}

		else
		{	
			i = lseek(file, 0, SEEK_END);

			while(i != -1 && counter != 10)
			{

				read(file, &wow, 1);	
				i --;
				lseek(file, i, SEEK_SET);
			

				if(wow == '\n')
				{
					counter ++;
				}

				characters++;
			}

			if (counter >= 10)
			{
				characters = characters - 2;
				char buffer[characters];
    			lseek(file, 2, SEEK_CUR);
		    	read(file, buffer, characters);
		    	write(1, buffer, characters);
			}

			else
			{
				characters = characters - 1;
				char buffer[characters];
				lseek(file, 0, SEEK_SET);
			    read(file, buffer, characters);
			    write(1, buffer, characters);
			}
		}
		close(file);
	}
}