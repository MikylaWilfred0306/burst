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

  /* setup input */
  int infd = STDIN_FILENO;
  if ((argc > 1) && (argv[1][0] != '-') && (argv[1][1] != '\0')) {
    infd = open(argv[1], O_RDONLY);
  }
  if (infd < 0) {
    perror("Input open error");
    return 1;
  }

  
  /* setup output */
		  int outfd = STDOUT_FILENO;

  char buf[BLOCK];
  while (1) {

    /* read a block */
    int bytesread = read(infd, buf, BLOCK);
    if ((bytesread == -1) && (errno == EINTR))
      continue;
    if (bytesread == -1) {
      perror("Read error on input file");
      return 1;
    }
    if (bytesread == 0)
      break;

    /* write a block */
    ssize_t byteswrote;
	int lines = countlines(argv[1]);
	int file_amount = lines % 500;
	int numthreads = file_amount + 1;
	
// allocate our the thread info
  struct threaddata_t* threadinfo = calloc(numthreads, sizeof(struct threaddata_t));
  if (!threadinfo) {
    fprintf(stderr, "Unable to allocate thread info\n");
    return 1;
  }	
	
	
	for (int j = 1; j <= file_amount; j++){  
	
		threadinfo[j].id = j;

		// use the thread to do some work
		pthread_create(&threadinfo[j].tid, NULL, process_thread, &threadinfo[j]);


		for (int i = 1; i < 500; i++){
		
		  if (argc > 2) {
			char str[15];
			sprintf(str, "%d", j); 
		  	strcat(argv[2] , str);
			outfd = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		  }
		  if (outfd < 0) {
			perror("Output open error");
			return 1;
		  }
		byteswrote = write(outfd, buf, bytesread);
		if (byteswrote == -1) {
		  perror("Output write error");
		  return 1;
		}
		}
	}
  }

  /* close input */
  if (infd != STDIN_FILENO)
    close(infd);

  /* close output */
  if (outfd != STDOUT_FILENO)
    close(outfd);
  
  return 0;
}
