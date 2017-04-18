/*

 currently based off of copyfile


 Copy from input file to output file
*/

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BLOCK 512

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
  if (argc > 2) {
    outfd = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  }
  if (outfd < 0) {
    perror("Output open error");
    return 1;
  }

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
    while (((byteswrote = write(outfd, buf, bytesread)) == -1) &&
	   (errno == EINTR))
      ;
    if (byteswrote == -1) {
      perror("Output write error");
      return 1;
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
