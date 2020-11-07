#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        	int file = 0;
    	    size_t size = 4096;
    	    char buffer[size];
    	    size_t n;

	    file = open(argv[1], O_RDONLY);

	    if(file < 0)
	    {
		      perror("Error");
	    }

	    else
	    {
      		int i;
      		int j = 0;
      		int counter = 0;
      		int counter_2 = 0;

		        while((n=read(file, buffer, 4096)) > 0)
	       	  {
    		 	      for(i = 0 ; i < n && counter < 10 ; i++)
    		 	      {
    		 		         if(buffer[i] == '\n')
    		 		         {
    		 			             counter ++;
    		 		         }
    		 	      }

          		 	if(counter == 10)
          		 	{
          		 		   write(1, buffer, i);
          		 		   break;
          		 	}

          		 	else
          		 	{
          		 		   write(1, buffer, n);
          		 	}
		         }

		write(1, "\n", 1);
		close(file);

     }
   }
    else
    {
      for(int o = 1 ; o < argc ; o++)
      {
            int file = 0;
      	    size_t size = 4096;
      	    char buffer[size];
      	    size_t n;

      	    file = open(argv[o], O_RDONLY);
      	    int name_count = sizeof(argv[o]) - 2;

      	    if(file < 0)
      	    {
      		      perror("Error");
      	    }

	           else
	            {
        	    	write(1, "==> ", 4);
        	    	write(1, argv[o], name_count);
        	    	write(1, " <==", 4);
        	    	write(1, "\n", 1);
            		int i;
            		int j = 0;
            		int counter = 0;
            		int counter_2 = 0;

		            while((n=read(file, buffer, 4096)) > 0)
	       	       {
		 	               for(i = 0 ; i < n && counter < 10 ; i++)
		 	                 {
		 		                    if(buffer[i] == '\n')
		 		                       {
		 			                           counter ++;
		 		                       }
		 	                 }

                		 	if(counter == 10)
                		 	{
                		 		write(1, buffer, i);
                		 		break;
                		 	}

                		 	else
                		 	{
                		 		write(1, buffer, n);
                		 	}

		              }

      		write(1, "\n", 1);
      		close(file);
	       }
       }
   }
   	write(1, "\n", 1);
    return 0;
}
