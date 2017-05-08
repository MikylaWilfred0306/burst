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
#include <getopt.h>
#include <archive.h>
#include <archive_entry.h>

#define BLOCK 512
extern char* optarg;
extern int optind, opterr, optopt;

struct option longopts[] = {

  { "output",           required_argument, NULL, 'o'},
  { "input",           required_argument, NULL, 'i'},
  
  0
};
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
          
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
       
        *(result + idx) = 0;
    }

    return result;
}


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


int main(int argc, char** argv) {

  int flag = 0;
  int option_result;
  int oc;
  int longindex;

char fileout1[256];
char filein[256];

  while ((oc = getopt_long(argc, argv, "i:o:", longopts, &longindex)) != -1) {

    // invalid options
    if (oc == '?') {
      fprintf(stderr, "-%c\n", oc);
      continue;
    }
 // where we are at in the options
    fprintf(stderr, "%d\t%c\n", longindex, oc);


    // action based argument reporter
    switch (oc) {
    
    case 'i':
      strcpy(filein, optarg);
      fprintf(stdout, "Got in: %s\n", optarg);
      break;

    case 'o':
      strcpy(fileout1, optarg);
      fprintf(stdout, "Got out: %s\n", optarg);
      break; 

    default:
      break;
    };
  }


//Defaulted outname for now
//strcpy(fileout1, "new");

char line[256];
char fileout[256];
char fileext[256];
char filenameout[256];

FILE* file = fopen(filein, "r");
int lines = countlines(filein);
char const* const fileName = filein;


int file_amount = lines / 500;
int numthreads = file_amount + 1;
int count = 1; 
int outfd = STDOUT_FILENO;
char buf[BLOCK];
int num = 0;

	
char** tokens;
	
tokens = str_split(fileout1, '.');

if (tokens)
{
	
	
	int i;
	for (i = 0; *(tokens + i); i++)
	{
		
		if(i == 0){
			strcpy(filenameout,*(tokens + i));
		} else {
			strcpy(fileext,*(tokens + i));
		}
	}
	free(tokens);
}


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
			strcpy(fileout,filenameout);
			char str[15];
			sprintf(str, "%d", j);
			strcat(fileout , str);
			strcat(fileout , ".");
			strcat(fileout , fileext);
 			printf("File Out Name = [%s]\n", fileout);
		
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
	
		threadinfo[num].id = num;

		// use the thread to do some work
		pthread_create(&threadinfo[num].tid, NULL, process_thread, &threadinfo[num]);
		FILE *fptr;
		strcpy(fileout,fileout1);
		char str[15];
		sprintf(str, "%d", num);
	
		strcat(fileout , str);
		strcat(fileout , ".");
		strcat(fileout , fileext);
		printf("File Out Name = [%s]\n", fileout);
		
		fptr = fopen(fileout , "rb+");
		if(fptr == NULL) //if file does not exist, create it
		{
			fptr = fopen(fileout, "wb");
		}
		int temp = file_amount * 500;
		int temp2 = lines - temp;
	  for (int i = 1; i <= temp2; i++){
				fgets(line, sizeof(line), file);
				// Open file in write mode
				fputs(line,fptr);
						
		}
	pthread_join(threadinfo[num].tid, NULL);    
		
  return 0;
}


