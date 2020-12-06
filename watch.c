#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

	if(argc == 2)
	{
		char* command = argv[1];
	    char* argument_list[] = {argv[1], NULL};

	 	for(int i = 0 ; i < 11 ; i++)
	 	{
			int status = execvp(command, argument_list);
			waitpid(-1, &status, 0);
		    sleep(2);
		}
	}
    return 0;
}