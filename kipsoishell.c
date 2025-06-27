// This is a simple C shell

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>          		//for fork(), execpv()
#include <sys/wait.h>			//for wait()
int main() {
	char *input = NULL;      	//will hold the input line
	size_t len = 0;     		//buffer size (getline will set it)
	ssize_t nread;

	//initializingg variables for the argument distincion
	char **args = NULL;  //pointer to array of strings
        int args_size = 0;
	int args_capacity = 8;    // start small, grow as needed
	
	// defining the size of the array using malloc
	args = malloc(args_capacity * sizeof(char *));
	if (!args) {
		perror("malloc failed\n");
		exit(1);
	}

	// Welcome message
	printf("Welcome to the Kipsoi s.h.e.l.l.\n");
	printf("===============Remember Always Have Fun========================\n");

	while (1){
		printf("kipsoish> ");
		nread = getline(&input, &len, stdin);

		if (nread == -1){
			printf("\nExiting shell.\n");
			break;   		//error or Ctrl+D
		}

		// Remove newline character
		if (input[nread - 1] == '\n'){
			input[nread - 1] = '\0';
		}

		// Exit command
		if (strcmp(input, "exit") == 0){
		       printf("\nThank you! Come back again!\n");	
			break;
			}

		// Breaking down user input to arguments and commands
		char *token = strtok(input, " ");
		while (token != NULL) {
			//Resize if full
			if (args_size >= args_capacity){
				args_capacity *= 2;
				args = realloc(args, args_capacity * sizeof(char *));
				if(!args) {
					perror("realloc failed");
					exit(1);
				}
			}

			args[args_size++] = token;
			token = strtok(NULL, " ");

		}

		args[args_size] = NULL;

		//see the parsed arguments
		printf("Parsed arguments:\n");
		for (int j = 0; j < args_size; j++){
			printf(" args[%d] = '%s'\n", j, args[j]);
		}
		// Debug: show what user typed
//		printf("You typed: %s\n", input);
		//=== Execute command ===
		pid_t pid = fork();
		if (pid == 0) {
			//child process
			if (execvp(args[0], args) == -1){
				perror("Command Failed");
			}
			exit(EXIT_FAILURE);   //only if exec fails
			free(args);
		} else if (pid > 0) {
			// Parent process
			wait(NULL);
		} else {
			perror("fork failed");
		}

		
		

	}
	
	free(input);	// free dynamically allocated buffer
	return 0;	
}
