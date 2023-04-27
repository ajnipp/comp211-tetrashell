// I, Bailey Van Wormer (730472595), pledge that I have neither given nor received unauthorized aid on this assignment.
// Collaborators: Alec Nipp (alecjn)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define MAX_TOKEN 10

int isMatchingCommand(char* input, char* command);

int main(int argc, char** argv) {

        char buff[FILENAME_MAX];
		
		char* welcomeMessage = "Welcome to...\n"
		"                              (\n"
		"  *   )          )            )\\ )    )       (   (\n"
		"` )  /(   (   ( /( (       ) (()/( ( /(    (  )\\  )\\\n"
		"( )(_)) ))\\  )\\()))(   ( /(  /(_)))\\())  ))\\((_)((_)\n"
		"(_(_()) /((_)(_))/(()\\  )(_))(_)) ((_)\\  /((_)_   _\n"
		"|_   _|(_))  | |_  ((_)((_)_ / __|| |(_)(_)) | | | |\n"
		"  | |  / -_) |  _|| '_|/ _` |\\__ \\| ' \\ / -_)| | | |\n"
		"  |_|  \\___|  \\__||_|  \\__,_||___/|_||_|\\___||_| |_|\n"
		"the ultimate Tetris quicksave hacking tool!\n";

		printf("%s", welcomeMessage);

		bool isRunning = true;	
		bool fileSelected = false;
		char filePathToHack[FILENAME_MAX];	
		char* prompt = "tetrashell> ";
		char* tokens[MAX_TOKEN];
		char* supportedCommands[] = {"exit", "modify", "rank", "check", "recover"};
		while (isRunning) {
			if (!fileSelected) {
        		printf("Enter the path to the quicksave you'd like to begin hacking: ");
        		if (fgets(filePathToHack, FILENAME_MAX, stdin) == NULL) {
					fprintf(stderr, "Error reading input with fgets\n");
					return EXIT_SUCCESS;
				}
				// Now we replace the newline character with the string terminator
				int len = strlen(filePathToHack);
				filePathToHack[len-1] = '\0';

				// Check if file path is accessible
				if (access(filePathToHack, F_OK) == 0) {
					fileSelected = true;
				} else {
					perror("Not a valid file path");
				}
			} else {
				// Now that we have a file selected, we can start asking for commands
				while (fileSelected) {
					printf("%s", prompt);	
					if (fgets(buff, FILENAME_MAX, stdin) == NULL) {
						fprintf(stderr, "Error reading input with fgets\n");
						return EXIT_SUCCESS;
					}
					int tokenCount = 0;
					char* token = strtok(buff, " ");
					while( tokenCount < MAX_TOKEN && token != NULL) {
						tokens[tokenCount] = token;
						//printf("token: %d\n", (int) *(token));
						token = strtok(NULL, " ");
						tokenCount++;
					}
					
					// If the user has entered nothing, just continue through the loop and prompt
					// for input again
					if (tokens[0][0] == '\n') {
						continue;
					}
					// Strip newline from last token
					char* lastToken = tokens[tokenCount - 1];
					lastToken[strlen(lastToken) - 1] = '\0';
					
					char* cmd = tokens[0];
					int commandCount = sizeof(supportedCommands) / sizeof(supportedCommands[0]);
					int numMatchingCommands = 0;
					char* matchingCommands[commandCount];
					for (int i = 0; i < commandCount; i++) {
						char* commandToMatch = supportedCommands[i];
						if (isMatchingCommand(cmd, commandToMatch)) {
							matchingCommands[numMatchingCommands] = commandToMatch;	
							numMatchingCommands++;
						}
					}

					// Check if no commands match input
					if (numMatchingCommands == 0) {
						printf("No matches for command %s: please try entering another command\n", cmd);
						continue;
					}
					
					// Check if input matches more than one command
					if (numMatchingCommands > 1) {
						printf("Multiple matches for command '%s'. Could be any of the following: \n", cmd);
						for (int i = 0; i < numMatchingCommands; i++) {
							printf("%s\n", matchingCommands[i]);
						}
						continue;
					}
					
					// printf("num matching: %d\n", numMatchingCommands);
					if (strcmp(cmd, "exit") == 0) {
						printf("Goodbye!\n");
						return EXIT_SUCCESS; 
					}

					
					
					
				}
			}
		}
}

int isMatchingCommand(char* input, char* command) {
		char c;
		int i = 0;
		while (( c = input[i] ) != '\0') {
			if ( c != command[i]) {
				return 0;
			}
			i++;
		}
		return 1;
}


