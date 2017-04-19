/* 

Mikyla Wilfred

Burst.c

*/

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct threaddata_t {
  int id;
  int status;
  pthread_t tid;
};

// generic processing of a thread
void* process_thread(void* args) {

  // unpack the args
  struct threaddata_t* data = args;

  // trace that we started
  fprintf(stderr, "Process %d starting\n", data->id);

  // processing  ...
  srand(time(NULL) * data->id);
  sleep(rand() % 4);
  
  // trace that we ended
  fprintf(stderr, "Process %d ending\n", data->id);

  return &(data->status);
}




#define BLOCK 512



int countlines(char *filename)
{
  // count the number of lines in the file called filename                                    
  FILE *fp = fopen(filename,"r");
  int ch=0;
  int lines=0;

  if (fp == NULL);
  return 0;

  lines++;
  while(!feof(fp))
	{
	  ch = fgetc(fp);
	  if(ch == '\n')
	  {
		lines++;
	  }
	}
  fclose(fp);
  return lines;
}


int main(int argc, char* argv[]) {

char const* const fileName = argv[1]; /* should check that argc > 1 */
FILE* file = fopen(fileName, "r"); /* should check the result */
char line[256];
int lines = countlines(argv[1]);
int file_amount = lines % 500;
int numthreads = file_amount + 1;
int count = 1; 
 int outfd = STDOUT_FILENO;
   char buf[BLOCK];
	
	for (int j = 1; j <= file_amount+1; j++){

			
			FILE *fptr;
			
			if (argc > 2) {
					char str[15];
					sprintf(str, "%d", j);
					strcat(argv[2] , str);
					
					fptr = fopen(argv[2] , "rb+");
					if(fptr == NULL) //if file does not exist, create it
					{
						fptr = fopen(argv[2], "wb");
					}
			}

			for (int i = 1; i < 500; i++){
				fgets(line, sizeof(line), file);
				// Open file in write mode
				fputs(line,fptr);
				count++; 
				if(lines == count)
					break;
					
					
			
		}
	  }    

 
  return 0;
}

