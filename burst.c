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
char fileout[256];
char fileout1[256];

strcpy(fileout1, argv[2]);


int lines = countlines(argv[1]);
int file_amount = lines / 500;
int numthreads = file_amount + 1;
int count = 1; 
int outfd = STDOUT_FILENO;
char buf[BLOCK];
int num = 0;
	for (int j = 0; j <= file_amount; j++){


			FILE *fptr;
			strcpy(fileout,fileout1);
			char str[15];
			sprintf(str, "%d", j);
			strcat(fileout , str);

			fptr = fopen(fileout , "rb+");
			if(fptr == NULL) //if file does not exist, create it
			{
				fptr = fopen(fileout, "wb");
			}

			for (int i = 1; i <= 500; i++){
				fgets(line, sizeof(line), file);
				// Open file in write mode
				fputs(line,fptr);
				
		}
		num++;
		
	  }
	  
	  
	  //This finishes out the file
		FILE *fptr;
		strcpy(fileout,fileout1);
		char str[15];
		sprintf(str, "%d", num);
		strcat(fileout , str);

		fptr = fopen(fileout , "rb+");
		if(fptr == NULL) //if file does not exist, create it
		{
			fptr = fopen(fileout, "wb");
		}
		int temp = num * 500;
		int temp2 = lines - temp;
	  for (int i = 0; i <= 500; i++){
				fgets(line, sizeof(line), file);
				// Open file in write mode
				fputs(line,fptr);
				if (temp2 == i)
					break;
				
		}
		
		
		
  return 0;
}

