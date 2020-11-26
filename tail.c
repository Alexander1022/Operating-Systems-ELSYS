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
	    char wow;
		size_t n;
		int counter = 0;
		int characters = 0;
		int std_characters = 0;
		int i = 0;
		char c;
		char ch[9999999999];
		
		while(read(STDIN_FILENO, &c, 1) > 0)
		{
		}

		i = lseek(STDIN_FILENO, 0, SEEK_END);

		while(i != -1 && counter != 10)
		{

			read(STDIN_FILENO, &wow, 1);	
			i --;
			lseek(STDIN_FILENO, i, SEEK_SET);
				

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
	    	lseek(STDIN_FILENO, 2, SEEK_CUR);
			read(STDIN_FILENO, buffer, characters);
			write(1, buffer, characters);
		}

		else
		{
			characters = characters - 1;
			char buffer[characters];
			lseek(STDIN_FILENO, 0, SEEK_SET);
			read(STDIN_FILENO, buffer, characters);
			write(1, buffer, characters);
		}
	} 

	else if(argc > 2)
	{
		for(int f = 1 ; f < argc ; f++)
		{
			if(strcmp(argv[f], "-") == 0)
			{
				int file = 0;
				file = open(argv[f], O_RDONLY);
				int name_count = strlen("standard input");
				write(1, "==> ", 4);
				write(1, "standard input", name_count);
				write(1, " <==", 4);
				write(1, "\n", 1);
				char wow;
				size_t n;
				int counter = 0;
				int characters = 0;
				int i = 0;

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

				if(f != argc - 1)
				{
					write(1, "\n", 1);
				}
			}

				else
				{
					int file = 0;
					char ch;
					file = open(argv[f], O_RDONLY);

					if(file == -1)
					{
						int name_count = strlen(argv[f]);
						write(2, "tail: cannot open '", 19);
						write(2, argv[f], name_count);
						perror("' for reading");
					}

					else if(read(file, &ch, 1) == -1)
					{
						int name_count = strlen(argv[f]);
						write(2, "tail: error reading '", 21);
						write(2, argv[f], name_count);
						perror("'");
					}

					else
					{
						int name_count = strlen(argv[f]);
						write(1, "==> ", 4);
						write(1, argv[f], name_count);
						write(1, " <==", 4);
						write(1, "\n", 1);
						char wow;
						size_t n;
						int counter = 0;
						int characters = 0;
						int i = 0;	
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

							if(close(file) == -1)
							{
								//tail: error reading 'b.txt': Input/output error
								int name_count = strlen(argv[f]);
								write(2, "tail: error reading '", 21);
								write(2, argv[f], name_count);
								perror("'");
							}

							if(f != argc - 1)
							{
								write(1, "\n", 1);
							}
					}
				}
		}
	}

	else if(argc == 2)
	{

		if(strcmp(argv[1], "-") == 0)
			{
				int name_count = strlen("standard input");
				write(1, "==> ", 4);
				write(1, "standard input", name_count);
				write(1, " <==", 4);
				write(1, "\n", 1);
				char wow;
				size_t n;
				int counter = 0;
				int characters = 0;
				int i = 0;
				int file = 0;
				file = open(argv[1], O_RDONLY);

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

		else
		{
			int file = 0;
			file = open(argv[1], O_RDONLY);
			char wow;
			size_t n;
			int counter = 0;
			int characters = 0;
			int i = 0;
			char ch;

			if(file == -1)
			{
				int name_count = strlen(argv[1]);
				write(2, "tail: cannot open '", 19);
				write(2, argv[1], name_count);
				perror("' for reading");
			}

			else if(read(file, &ch, 1) == -1)
			{
				int name_count = strlen(argv[1]);
				write(2, "tail: error reading '", 21);
				write(2, argv[1], name_count);
				perror("'");
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

			if(close(file) == -1)
			{
				//tail: error reading 'b.txt': Input/output error
				int name_count = strlen(argv[1]);
				write(2, "tail: error reading '", 21);
				write(2, argv[1], name_count);
				perror("'");
			}
	}
	}
}