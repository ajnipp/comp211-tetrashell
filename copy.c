// I, Bailey Van Wormer (730472595), pledge that I have neither given nor
// received unauthorized aid on this assignment. Collaborators: Alec Nipp
// (alecjn) & Bailey Van Wormer (bvanwo)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TOKEN 10

void exit_shell();
int isMatchingCommand(char* input, char* command);

int main(int argc, char** argv) {
  char buff[FILENAME_MAX];

  char* welcomeMessage =
      "Welcome to...\n"
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
      filePathToHack[len - 1] = '\0';

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
        while (tokenCount < MAX_TOKEN && token != NULL) {
          tokens[tokenCount] = token;
          // printf("token: %d\n", (int) *(token));
          token = strtok(NULL, " ");
          tokenCount++;
        }

        // If the user has entered nothing, just continue through the loop and
        // prompt for input again
        if (tokens[0][0] == '\n') {
          continue;
        }
        // Strip newline from last token
        char* lastToken = tokens[tokenCount - 1];
        lastToken[strlen(lastToken) - 1] = '\0';

        char* cmd = tokens[0];
        int commandCount =
            sizeof(supportedCommands) / sizeof(supportedCommands[0]);
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
          printf(
              "No matches for command %s: please try entering another "
              "command\n",
              cmd);
          continue;
        }

        // Check if input matches more than one command
        if (numMatchingCommands > 1) {
          printf(
              "Multiple matches for command '%s'. Could be any of the "
              "following: \n",
              cmd);
          for (int i = 0; i < numMatchingCommands; i++) {
            printf("%s\n", matchingCommands[i]);
          }
          continue;
        }

        // Since the input matches one command, we can replace the input with
        // the full text of that command for comparison
        cmd = matchingCommands[0];

        if (strcmp(cmd, "exit") == 0) {
          exit_shell();
        }

        if (strcmp(cmd, "modify") == 0) {
          // need to implement
          // use execv to call program to run (./modify)
          // first argument is what to run, second argument is list of arguments
          // Also need to use fork to override while loop with ./modify
          // run execv in child
          // use wait in parent to make it wait until child finishes

	  // Check if number of args is correct
	  if (tokenCount != 3) {
		 printf("Command 'modify' needs 2 arguments but %d were provided!\n", tokenCount - 1);
		 continue;
	}	 
	  // NULL-terminated args list for execve
	  char* args[5] = {"./modify", tokens[1], tokens[2], filePathToHack, NULL};
	  
          pid_t pid = fork();

            if (pid == 0) {
            // we know we are in our child process since fork returns 0 for
            // child use execve here to execute ./modify
            	execv("./modify", args);
			exit(0);
	    } else {
			wait(NULL);
			exit(0);
		  }
        } else if (strcmp(cmd, "rank") == 0) {
	  if (tokenCount != 3) {
	  	printf("Command 'rank' needs 2 arguments but %d were provided!\n", tokenCount - 1);
		continue;
	  }
	  char* args2[5] = {"./rank", tokens[1], tokens[2], "uplink", NULL};
	  int my_pipes[2];
  	  pipe(my_pipes);	  

	  pid_t pid2 = fork();
	  
	  if (pid2 == 0) {
		close(my_pipes[1]);
		dup2(my_pipes[0], 0);
		execv("./rank", args2);
		exit(0);
	  } else {
		close(my_pipes[0]);
	        write(my_pipes[1], filePathToHack, strlen(filePathToHack));
		close(my_pipes[1]);	
	  	wait(NULL);
		exit(0);
	  }
        } else if (strcmp(cmd, "recover") == 0) {
	  if (tokenCount != 2) {
	  	printf("Command 'recover' needs 1 argument but %d were provided!\n", tokenCount - 1);
		continue;
	  }

	  char* args3[3] = {"./recover", tokens[1], NULL};

	  pid_t pid3 = fork();

	  if (pid3 == 0) {
	  	execv("./recover", args3);
		exit(0);
	  } else {
	  	wait(NULL);
		exit(0);
	  }
        } else if (strcmp(cmd, "check") == 0) {
	  if (tokenCount != 1) {
	  	printf("Command 'check' takes no arguments, but you provided %d\n.", tokenCount - 1);
		continue;
	  }

	  char* args4[3] = {"./check", filePathToHack, NULL};

	  pid_t pid4 = fork();

	  if (pid4 == 0) {
	  	execv("./check", args4);
		exit(0);
	  } else {
		wait(NULL);
	        exit(0);	
		}
        }
      }
    }
  }
}

void exit_shell() {
  printf("Goodbye!\n");
  exit(0);
}
int isMatchingCommand(char* input, char* command) {
  char c;
  int i = 0;
  while ((c = input[i]) != '\0') {
    if (c != command[i]) {
      return 0;
    }
    i++;
  }
  return 1;
}
