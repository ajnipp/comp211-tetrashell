// I, Bailey Van Wormer (730472595), pledge that I have neither given nor
// received unauthorized aid on this assignment. Collaborators: Alec Nipp
// (alecjn) & Bailey Van Wormer (bvanwo)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "tetris.h"

#define MAX_TOKEN 10

void exit_shell();
int isMatchingCommand(char* input, char* command);
int readStateFromFile(TetrisGameState* state, char* fileName);

int main(int argc, char** argv) {
  char buff[FILENAME_MAX];

  char* welcomeMessage =
      "Welcome to...\n"
      "\033[38;5;208m"
      "                              (\n"
      "  *   )          )            )\\ )    )       (   (\n"
      "` )  /(   (   ( /( (       ) (()/( ( /(    (  )\\  )\\\n"
      "( )(_)) ))\\  )\\()))(   ( /(  /(_)))\\())  ))\\((_)((_)\n"
      "(_(_()) /((_)(_))/(()\\  )(_))(_)) ((_)\\  /((_)_   _\n"
      "\033[38;5;51m"
      "|_   _|(_))  | |_  ((_)((_)_ / __|| |(_)(_)) | | | |\n"
      "  | |  / -_) |  _|| '_|/ _` |\\__ \\| ' \\ / -_)| | | |\n"
      "  |_|  \\___|  \\__||_|  \\__,_||___/|_||_|\\___||_| |_|\n"
      "\033[0m"
      "the ultimate Tetris quicksave hacking tool!\n";

  printf("%s", welcomeMessage);

  char* user = getlogin();
  char name[64];

  // Default value of host name if it can't be fetched
  if (gethostname(name, 64)) {
    char* tshell = "TShell";
    strcpy(name, tshell);
  }

  bool isRunning = true;
  bool fileSelected = false;
  char filePathToHack[FILENAME_MAX];
  FILE* filePointerToHack;
  char* prompt = "tetrashell> ";
  char* tokens[MAX_TOKEN];
  char* supportedCommands[] = {"exit",   "modify", "rank", "check", "recover",
                               "switch", "help",   "info", "undo"};
  TetrisGameState currentGameState;
  TetrisGameState oldGameState;  // tracking it for undo
  bool isModified = false;

  char truncatedFileName[6];

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
        if (readStateFromFile(&currentGameState, filePathToHack)) {
          continue;
        }
        strncpy(truncatedFileName, filePathToHack, 5);
        truncatedFileName[5] = '\0';

        fileSelected = true;

      } else {
        perror("Not a valid file path");
      }
    } else {
      while (fileSelected) {
        printf("\033[38;5;51m%s@%s[%s...][%u/%u]>\033[0m ", user, name, truncatedFileName,
               currentGameState.score, currentGameState.lines);
        if (fgets(buff, FILENAME_MAX, stdin) == NULL) {
          fprintf(stderr, "Error reading input with fgets\n");
          return EXIT_SUCCESS;
        }
        int tokenCount = 0;
        char* token = strtok(buff, " ");
        while (tokenCount < MAX_TOKEN && token != NULL) {
          tokens[tokenCount] = token;
          token = strtok(NULL, " ");
          tokenCount++;
        }

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

        if (numMatchingCommands == 0) {
          printf(
              "No matches for command %s: please try entering another "
              "command\n",
              cmd);
          continue;
        }

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

        cmd = matchingCommands[0];

        if (strcmp(cmd, "exit") == 0) {
          exit_shell();
        }
        pid_t pid;
        char** args;
        if (strcmp(cmd, "modify") == 0) {
          if (tokenCount != 3) {
            printf("Command 'modify' needs 2 arguments but %d were provided!\n",
                   tokenCount - 1);
            continue;
          }

          // Save the current state as the old state before modifying
          if (readStateFromFile(&oldGameState, filePathToHack)) {
            continue;
          }

          char* args[] = {"./modify", tokens[1], tokens[2], filePathToHack,
                          NULL};

          pid = fork();

          if (pid == 0) {
            execv("./modify", args);
            exit(0);
          } else {
            wait(NULL);
          }
          isModified = true;
          readStateFromFile(&currentGameState, filePathToHack);

        } else if (strcmp(cmd, "rank") == 0) {
          if (tokenCount != 3) {
            if (tokenCount != 2) {
              printf(
                  "Command 'rank' needs at least 1 argument, but %d were "
                  "provided!\n",
                  tokenCount - 1);
              continue;
            } else {
              // sets tokens[2] value for quick-rank
              tokens[2] = "10";
            }
          }

          char* args[] = {"./rank", tokens[1], tokens[2], "uplink", NULL};
          int my_pipes[2];
          pipe(my_pipes);

          pid = fork();

          if (pid == 0) {
            close(my_pipes[1]);
            dup2(my_pipes[0], 0);
            execv("./rank", args);
            exit(0);
          } else {
            close(my_pipes[0]);
            write(my_pipes[1], filePathToHack, strlen(filePathToHack));
            close(my_pipes[1]);
            wait(NULL);
          }
        } else if (strcmp(cmd, "recover") == 0) {
          if (tokenCount != 2) {
            printf("Command 'recover' needs 1 argument but %d were provided!\n",
                   tokenCount - 1);
            continue;
          }

          char* args[] = {"./recover", tokens[1], NULL};

          pid = fork();

          if (pid == 0) {
            execv("./recover", args);
            exit(0);
          } else {
            wait(NULL);
          }
        } else if (strcmp(cmd, "check") == 0) {
          if (tokenCount != 1) {
            printf("Command 'check' takes no arguments, but you provided %d\n.",
                   tokenCount - 1);
            continue;
          }

          char* args[] = {"./check", filePathToHack, NULL};

          pid = fork();

          if (pid == 0) {
            execv("./check", args);
            exit(0);
          } else {
            wait(NULL);
          }
        } else if (strcmp(cmd, "switch") == 0) {
          if (tokenCount != 2) {
            printf("Command 'switch' needs 1 argument but %d were provided!\n",
                   tokenCount - 1);
            continue;
          }
          char* newFilePath = tokens[1];
          // Check if file path is accessible
          if (access(newFilePath, F_OK) == 0) {
            printf("Switched current save from '%s' to '%s'\n", filePathToHack,
                   newFilePath);
            strcpy(filePathToHack, newFilePath);
            strncpy(truncatedFileName, filePathToHack, 5);
            truncatedFileName[6] = '\0';
            // Update current state object
            readStateFromFile(&currentGameState, filePathToHack);
          } else {
            perror("Not a valid file path");
            continue;
          }
        } else if (strcmp(cmd, "help") == 0) {
          if (tokenCount != 2) {
            printf("Command 'help' needs 1 argument but %d were provided!\n",
                   tokenCount - 1);
            continue;
          }

          if (strcmp(tokens[1], "exit") == 0) {
            printf(
                "This command calls the 'exit' program with no additonal "
                "arguments, causing the program to exit.\n");
          } else if (strcmp(tokens[1], "modify") == 0) {
            printf(
                "This command calls the 'modify' program, passing in either "
                "'score' or 'lines' as the first argument.\nIf you wish to "
                "modify the score of the quicksave, pass in score, similarly "
                "passing in lines will modify the lines of the quicksave.\nThe "
                "second argument is the count which you wish to modify the "
                "score or line to be.\nModify will then modify your quicksave "
                "to update the score or lines to your passed in count "
                "value.\n");
          } else if (strcmp(tokens[1], "rank") == 0) {
            printf(
                "This command calls the 'rank' progam.\nYou can choose to pass "
                "in either 'score' or 'lines' as the first argument, this is "
                "the criteria by which rank will list quicksaves by.\nThe "
                "second argument is the number of ranked quicksaves you would "
                "like to see printed.\n");
          } else if (strcmp(tokens[1], "check") == 0) {
            printf(
                "This command calls the 'check' program with the current "
                "quicksave to verify if it will pass legitimacy checks.\nCheck "
                "takes no arguments.\n");
          } else if (strcmp(tokens[1], "switch") == 0) {
            printf(
                "This command switches the current quicksave to the new "
                "quicksave found at the path you enter as the argument.\n");
          } else if (strcmp(tokens[1], "help") == 0) {
            printf(
                "This command provides information on how to use the various "
                "commands in this tool!\nEnter the command as the "
                "argument.\nSome possible commands are 'exit', 'check', "
                "'modify', etc.\n");
          } else if (strcmp(tokens[1], "info") == 0) {
            printf(
                "This command provides information on the current quicksave, "
                "including the path to the quicksave, as well as its number of "
                "scores and lines.\nTo use this command, simply enter the "
                "'info' command with no arguments.\n");
          } else if (strcmp(tokens[1], "recover") == 0) {
            printf(
                "This command calls the 'recover' program and returns a list "
                "of recovered quicksaves from the disk image file path "
                "specified by the passed in argument.\nThis command only takes "
                "in 1 argument which is the path to the disk image file you "
                "wish to recover quicksaves from.\n");
          } else if (strcmp(tokens[1], "undo") == 0) {
            printf(
                "This command reverts the last modify command executed.\nFor "
                "example, if score was previously 5, and you called modify "
                "score 100, then called undo, the score would be reverted from "
                "100 back to 5.\nundo does not take any arguments.\n");
          } else {
            printf(
                "Command not recognized, try a new command.\nSee 'help help' "
                "for a few possible commands.\n");
          }
        } else if (strcmp(cmd, "info") == 0) {
          if (tokenCount != 1) {
            printf("Command 'info' needs 0 arguments but %d were provided!\n",
                   tokenCount - 1);
            continue;
          }

          if (readStateFromFile(&currentGameState, filePathToHack)) {
            continue;
          }

          printf("Current savefile: %s\n", filePathToHack);
          printf("Score: %u\n", currentGameState.score);
          printf("Lines: %u\n", currentGameState.lines);
        } else if (strcmp(cmd, "undo") == 0) {
          if (tokenCount != 1) {
            printf("Command 'undo' needs 0 arguments but %d were provided!\n",
                   tokenCount - 1);
            continue;
          }

          // Check if there haven't been any modifications
          if (isModified == false) {
            printf("No modifications have been made to undo!\n");
            continue;
          }

          FILE* fp = fopen(filePathToHack, "w");

          if (fp == NULL) {
            printf("Error opening file '%s'!\n", filePathToHack);
            fclose(fp);
            continue;
          }

          // Write old game state to save file
          if (fwrite(&oldGameState, sizeof(TetrisGameState), 1, fp) != 1) {
            printf("Error writing to file with fwrite!\n");
            fclose(fp);
            continue;
          }

          fclose(fp);

          printf("Last modification to '%s' has been undone.\n",
                 filePathToHack);
          readStateFromFile(&currentGameState, filePathToHack);
          isModified = false;
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

int readStateFromFile(TetrisGameState* state, char* fileName) {
  FILE* fp = fopen(fileName, "r");

  if (fp == NULL) {
    printf("Error opening file '%s'!\n", fileName);
    fclose(fp);
    return 1;
  }

  if (fread(state, sizeof(TetrisGameState), 1, fp) != 1) {
    printf("Error reading file with fread!\n");
    fclose(fp);
    return 1;
  }

  fclose(fp);
  return 0;
}
