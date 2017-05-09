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
{ "numberLines",           required_argument, NULL, 'n'},
{ "all",           required_argument, NULL, 'a'},
  
	
	{ "help",               no_argument,       NULL, 'h'},
    { "version",            no_argument,       NULL, 'V'},
    { "quiet",            no_argument,       NULL, 'q'},
	
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

  // processing  ...
  srand(time(NULL) * data->id);
  sleep(rand() % 4);

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
  int inflag = 0;
  int stdflag = 0;
  int outflag = 0;
  int option_result;
  int oc;
  int longindex;
  int linesAmount = 500;

char fileout1[256];
char filein[256];

  while ((oc = getopt_long(argc, argv, "i:o:n:hVqa:", longopts, &longindex)) != -1) {

    // invalid options
    if (oc == '?') {
      fprintf(stderr, "-%c\n", oc);
      continue;
    }
 

    // action based argument reporter
    switch (oc) {
    
    case 'i':
      strcpy(filein, optarg);
	  inflag = 1;
      break;

    case 'o':
      strcpy(fileout1, optarg);
	  outflag = 1;
      break; 
	
	case 'a':
      strcpy(filein, optarg);
	  inflag = 1;
      strcpy(fileout1, optarg);
	  outflag = 1;
      break;		
			
	case 'n':
	  linesAmount = strtoumax(optarg, NULL, 10);
      break; 
			
	case 'h':
	 fprintf(stdout, "NAME\n./burst This function breaks apart a file based on number of lines.\nUsage: ./bash [OPTION]...\nThis function breaks apart a file inputed in the line to multiple files based on the output name, also supplied in line. \n-a, --all=FILE\nThe argument is used as both the input file and the output file.\n-h, --help\nThis option gives the usage of the program.\n-i, --input=FILE\nThe argument is used as the original file for the input.\n-n, --numberLines=NUMBER\nThe argument is used for the number of lines for each output file. The program defaults to 500 if there is no option.\n-o, --output=FILE\nThe argument is used as the base name for the output file.\n-q, --quiet\nThis option removes all messages from being shown. \n-V, --version\nThis option gives the version of the program.\nEXAMPLES\n./bash -i test.txt -o new.txt\nReads from test.txt and prints to new-0.txt, new-1.txt... 500 lines a file.\n./bash -a test.txt\nReads from test.txt and prints to test-0.txt, test-1.txt... 500 lines a file.\n ./bash -i test.txt -o new.txt -n 250\nReads from test.txt and prints to test-0.txt, test-1.txt... 250 lines a file. \n");
      return 1;
			
	case 'V':
	  fprintf(stdout, "Version: 1.0\n");
      return 1;

	case 'q':
	  stdflag = 1;
      break;
		
			
    default:
      break;
    };
  }

if(inflag == 0){
	fprintf(stdout, "File in is mandatory. Use -i\n");
     return 1;
}
if(outflag == 0){
	fprintf(stdout, "File out is mandatory. Use -o\n");
     return 1;
}

char line[256];
char fileout[256];
char fileext[256];
char filenameout[256];

FILE* file = fopen(filein, "r");
int lines = countlines(filein);
char const* const fileName = filein;
	

if (stdflag == 0){	
	fprintf(stdout, "Number of lines: %d\n", linesAmount);
}
int file_amount = lines / linesAmount;
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
			strcat(fileout , "-");
			strcat(fileout , str);
			strcat(fileout , ".");
			strcat(fileout , fileext);
	if (stdflag == 0){
 			printf("File Out Name = [%s]\n", fileout);
	}
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
	
		strcat(fileout , "-");
		strcat(fileout , str);
		strcat(fileout , ".");
		strcat(fileout , fileext);
		if (stdflag == 0){
		printf("File Out Name = [%s]\n", fileout);
	}
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


