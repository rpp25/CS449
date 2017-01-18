//author rpp25. This program creates a shell that accepts Unix commands
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	char delimit[20]=  " \n\t()<>|&;";
	char input[500];
	char* token;

	while (1){
		FILE *fpoint;
		printf("$ "); //start with $ so shell is ready to accept commands
		if (!fgets(input, 500, stdin)) break;
		char* write = strstr(input,">"); //finds character used for writing
		char* append = strstr(input,">>"); //finds character used for appending
		char* redirect = strstr(input,"<"); //finds character used for redirecting
		token = strtok(input, delimit); //tokenize the input using the delimiters
		char *comm[10];
		comm[0] = token;
		int i;
		for (i = 0; i <= 10; i++)
		{
			token = strtok(NULL, delimit);
			comm[i+1] = token;
			if (token == NULL)
			{
				break;
			}
		}
		if (strcmp(comm[0], "exit") == 0)
		{
			break;
		}else if (strcmp(comm[0], "cd") == 0){
			char *dest = comm[1]; //path stored in comm[1]
			if (dest == NULL)
			{
				fprintf(stderr,"Invalid argument for CD"); //displays error if cd has no argument
			}
			else
			{
				chdir(dest);
			}
		}else{
			pid_t child;
			int status; //child's exit status
			pid_t idChild; //process id of child
			char *ch= ">";
			if ((child = fork()) == 0) //child process
			{
				if (append != NULL)
				{
					//char *file = comm[1];
					char *file = comm[i];
					fpoint = freopen(file, "a+", stdout); //opens file for appending
					//comm[i]=NULL;
					execvp(comm[0], comm);
					close(fpoint); //close the file
					exit(1);

				}else if (write != NULL){
					char *file = comm[i];
					fpoint = freopen(file, "r+", stdout); //opens an existing file for writing
					//comm[i] = NULL;
					execvp(comm[0], comm);
					close(fpoint);
					exit(1);

				}else if (redirect != NULL){
					char *file = comm[i];
					fpoint = freopen(file, "r", stdin); //opens a file for reading
					//comm[i] = NULL;
					execvp(comm[0], comm);
					exit(1);
				}else{
					execvp(comm[0], comm);
					exit(1);
				}
			}
			else { //parent
				if (child /= (pid_t)(-1)) { //process id of -1 returns an error
					idChild = wait(&status); //wait for the child process to execute and terminate
				}
			}
		}
	}
	return 0;
}

