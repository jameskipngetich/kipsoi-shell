// This is a simple C shell

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char *input = NULL;      	//will hold the input line
	size_t len = 0;     		//buffer size (getline will set it)
	ssize_t nread;

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

		// Debug: show what user typed
		printf("You typed: %s\n", input);

	}

	free(input); 		// free dynamically allocated buffer
	return 0;
}
