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

#define BLOCK 512


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





int countlines(char *filename)
{

FILE *fp;
    int count = 0;  // Line counter (result)
	
	 char c;  // To store a character read from file
// Open the file
    fp = fopen(filename, "r");
 
    // Check if file exists
    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }
 
    // Extract characters from file and store in character c
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
 
  return count;
}


int main(int argc, char* argv[]) {
int lines = countlines(argv[1]);
char const* const fileName = argv[1]; /* should check that argc > 1 */
FILE* file = fopen(fileName, "r"); /* should check the result */
char line[256];
char fileout[256];
char fileout1[256];

strcpy(fileout1, argv[2]);



int file_amount = lines / 500;
int numthreads = file_amount + 1;
int count = 1; 
int outfd = STDOUT_FILENO;
char buf[BLOCK];
int num = 0;



// allocate our the thread info
struct threaddata_t* threadinfo = calloc(numthreads, sizeof(struct threaddata_t));
if (!threadinfo) {
	fprintf(stderr, "Unable to allocate thread info\n");
	return 1;
}
	for (int j = 0; j < file_amount; j++){
			threadinfo[j].id = j;

			// use the thread to do some work
			pthread_create(&threadinfo[j].tid, NULL, process_thread, &threadinfo[j]);

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
		
		pthread_join(threadinfo[j].tid, NULL);    
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
		int temp = file_amount * 500;
		int temp2 = lines - temp;
		 fprintf(stdout, "BLAHHHH %d\n", temp2);
	  for (int i = 1; i <= temp2; i++){
				fgets(line, sizeof(line), file);
				// Open file in write mode
				fputs(line,fptr);
							
		}
		
		
		
  return 0;
}

