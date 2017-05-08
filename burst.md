NAME
    ./burst This function breaks apart a file based on number of lines.
SYNOPSIS
  Usage: ./bash [OPTION]...
          
DESCRIPTION
    This function breaks apart a file inputed in the line to multiple files based on the output name, also supplied in line. 
	
	-a, --all=FILE
		The argument is used as both the input file and the output file. 
		
	-h, --help
		This option gives the usage of the program. 
		
	-i, --input=FILE
		The argument is used as the original file for the input.
		
	-n, --numberLines=NUMBER
		The argument is used for the number of lines for each output file. The program defaults to 500 if there is no option.
	
	-o, --output=FILE
		The argument is used as the base name for the output file.
		
	-q, --quiet
		This option removes all messages from being shown. 
		
	-V, --version
		This option gives the version of the program. 

EXAMPLES
    ./bash -i "test.txt" -o "new.txt" 
		Reads from test.txt and prints to new-0.txt, new-1.txt... 500 lines a file. 
		
    ./bash -a "test.txt" 
		Reads from test.txt and prints to test-0.txt, test-1.txt... 500 lines a file. 
		
    ./bash -i "test.txt" -o "new.txt" -n 250 
		Reads from test.txt and prints to test-0.txt, test-1.txt... 250 lines a file. 

AUTHOR
   Mikyla Wilfred
   mikyla.wilfred@gmail.com
   
COPYRIGHT
    Specify your copyright information.