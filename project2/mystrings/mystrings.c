/* author: rpp25. Raj Patel 
   This program is the first part of the project: mystrings*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[]){
	int count = 0;	
	unsigned int x;
	FILE *file;
	file = fopen(argv[1], "r+"); //opens the file reading
	char str[4]; //strings must be at least 4 characters long
	
	if (file) {
		while ((x = getc(file)) != EOF){ //while file is open
			if (32 <= x && x <= 126){
				if (count < 5){
    				str[count] = x;
    				count++;
    			}else if (count==5){
    				printf("\n%s", str);
    				putchar(x);
    				count++;
    			}else
    				putchar(x);

        	}else{
        		count=0;
        	}
    	}	
    	fclose(file);
	}else
		printf("Error");
	return 0;
}