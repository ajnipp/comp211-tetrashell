// I, Bailey Van Wormer (730472595), pledge that I have neither given nor received unauthorized aid on this assignment.
// Collaborators: Alec Nipp (alecjn)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define MAX_TOKEN 10

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
						token = strtok(NULL, " ");
						tokenCount++;
					}
					// Strip newline from last token
					char* lastToken = tokens[tokenCount - 1];
					lastToken[strlen(lastToken) - 1] = '\0';

					if (tokenCount == 0) {
						continue;
					}
					char* cmd = tokens[0];
					if (strcmp(cmd, "exit") == 0) {
						printf("Goodbye!\n");
						return EXIT_SUCCESS; 
					}
				}
			}
		}
}

