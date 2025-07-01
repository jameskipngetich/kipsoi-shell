// This is a simple C shell

#include<stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>          		//for fork(), execpv()
#include <sys/wait.h> 			//for wait()
#include <limits.h>			//for PATH_MAX
//for history recording
#define HISTORY_MAX 100
int main() {
	char *input = NULL;      	//will hold the input line
	size_t len = 0;     		//buffer size (getline will set it)
	char *history[HISTORY_MAX];
	int history_count = 0;
	
	// Welcome message
	printf("Welcome to the Kipsoi s.h.e.l.l.\n");
	printf("===============Remember Always Have Fun========================\n");

	while (1){

		//creating the prompt message

		char cwd[PATH_MAX];
		getcwd(cwd, sizeof(cwd));
		char prompt[PATH_MAX + 5];
		
		snprintf(prompt, sizeof(prompt), "%s:) ",cwd); // formart prompt
		input = readline(prompt);
		if (!input) {
			printf("\nExiting shell.\n");
			break;
		}

		//save to readline's history
		if (*input) {
			add_history(input);
		}

		//record history of commands
		if (history_count < HISTORY_MAX) {
			history[history_count++] = strdup(input);
		}
/*
		if (nread == -1){
			printf("\nExiting shell.\n");
			break;   		//error or Ctrl+D
		}

		// Remove newline character
		if (input[nread - 1] == '\n'){
			input[nread - 1] = '\0';
		}
*/
		// Exit command
		if (strcmp(input, "exit") == 0){
		       printf("\nThank you! Come back again!\n");	
			break;
			}
		
		// Running a command from history
		if (input[0] == '!' && history_count > 0) {
			int cmd_num = atoi(&input[1]);  //convert "!2" to 2

			if (cmd_num <= 0 || cmd_num > history_count) {
				printf("kipsoish: no such command in history\n");
				continue;
			}
			// Replace input with the historical command
			free(input);  // free old line buffer
			input = strdup(history[cmd_num - 1]); 
			len = strlen(input) + 1; // update length for correctness

			printf("Running: %s\n", input);
		}

		// Breaking down user input to arguments and commands
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

		/*
		/see the parsed arguments
		printf("Parsed arguments:\n");
		for (int j = 0; j < args_size; j++){
			printf(" args[%d] = '%s'\n", j, args[j]);
		}
		// Debug: show what user typed
        // printf("You typed: %s\n", input);
		*/
		//=== Execute command ===
		// Handle internal command: cd
		if (strcmp(args[0], "cd") == 0){
			if (args[1] == NULL) {
				fprintf(stderr, "kipsoish: expected argument to  \"cd\"\n");
			} else{
				if (chdir(args[1]) != 0){
					perror("kipsoish");
				}
			}

			free(args);
			continue;  // skip fork/exec for internal command
		}

		//checking for the history command
		if (strcmp(args[0], "history") == 0) {
			for (int i = 0; i < history_count; i++){
				printf("%d: %s\n", i + 1, history[i]);
			}
			free(args);
			continue;
		}

		//creating a child process using fork()
		pid_t pid = fork();
		if (pid == 0) {
			//child process
			if (execvp(args[0], args) == -1){
				fprintf(stderr, "kipsoish: command not found: %s\n", args[0]);
			}
			exit(EXIT_FAILURE);   //only if exec fails
			
		} else if (pid > 0) {
			// Parent process
			wait(NULL);
		} else {
			perror("fork failed");
		}

		free(args);
		

	}
	
	free(input);	// free dynamically allocated buffer
	for (int i = 0; i < history_count; i++) {
		free(history[i]);  // free each saved command
	}
	return 0;	

}
