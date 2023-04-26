// I, Bailey Van Wormer (730472595), pledge that I have neither given nor received unauthorized aid on this assignment.
// Collaborators: Alec Nipp (alecjn)

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

        char *input;
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
        printf("Enter the path to the quicksave you'd like to begin hacking: ");

        input = fgets(buff, FILENAME_MAX, stdin);
        if (input == NULL) {
                fprintf(stderr, "error with fgets");
                return EXIT_SUCCESS;
        }

        printf("%s\n", input);


}

