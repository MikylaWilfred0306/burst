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

int main(int argc, char* argv[]) {

	FILE *ptr_readfile;
	FILE *ptr_writefile;
	char line [500]; 
	char* fileoutputname[80];
	fileoutputname* = argv[2]; 
	int filecounter=1, linecounter=1;
	if ((argc > 1) && (argv[1][0] != '-') && (argv[1][1] != '\0')) {
	   ptr_readfile = fopen(argv[1],"r");
  	}
	if (!ptr_readfile)
		perror("Input open error");
		return 1;

	sprintf(fileoutputname, "file_part%d", filecounter);
	ptr_writefile = fopen(fileoutputname, "w");

	while (fgets(line, sizeof line, ptr_readfile)!=NULL) {
		char fileoutputnamenew[80];
        fileoutputnamenew = strcat(fileoutputnamenew, line);
		FILE *fptr;
		fptr = fopen(fileoutputnamenew, "rb+");
		if(fptr == NULL) //if file does not exist, create it
		{
			fptr = fopen(fileoutputnamenew, "wb");
		}
		if (linecounter == 5) {
			fclose(ptr_writefile);
			linecounter = 1;
			filecounter++;
			sprintf(fileoutputnamenew, "file_part%d", filecounter);
			ptr_writefile = fopen(fileoutputnamenew, "w");
			if (!ptr_writefile)
				return 1;
		}
		fprintf(ptr_writefile,"%s\n", line);
		linecounter++;
	}
	fclose(ptr_readfile);
	return 0;
}

