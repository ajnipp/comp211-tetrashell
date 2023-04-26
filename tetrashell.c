// I, Bailey Van Wormer (730472595), pledge that I have neither given nor received unauthorized aid on this assignment.
// Collaborators: Alec Nipp (alecjn)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

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
		"the ultimate Tetris quicksave hacking tool!\n\0";

		printf("%s", welcomeMessage);

		bool isRunning = true;	
		bool fileSelected = false;
		char filePathToHack[FILENAME_MAX];	
		while (isRunning) {
			if (!fileSelected) {
        		printf("Enter the path to the quicksave you'd like to begin hacking: ");
        		if (fgets(buff, FILENAME_MAX, stdin) == NULL) {
					fprintf(stderr, "Error reading input with fgets\n");
					return EXIT_SUCCESS;
				}
				strcpy(filePathToHack, buff);
				// Now we replace the newline character with the string terminator
				int len = strlen(filePathToHack);
				filePathToHack[len-1] = '\0';

				if (access(filePathToHack, F_OK) == 0) {
					// File path works, so assign it to our file path variable
					fileSelected = true;
				} else {
					perror("Not a valid file path");
				}
			} else {
				while (fileSelected) {

				}
			}
		}
}

