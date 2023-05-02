# Welcome to our A5 Documentation
Welcome to our A5 project for COMP 211! We sincerely hope you enjoy using our tool!

  

This README provides some background information about our tool.

If you would like to learn more about what each command does, use the help command!

  

Teammates: Bailey Van Wormer (bvanwo) & Alec Nipp (alecjn)

  

## Supported Commands:

 - **modify**
	 - This command calls the 'modify' program, passing in either 'score' or 'lines' as the first argument.
     - If you wish to modify the score of the quicksave, pass in score, similarly passing in lines will modify the lines of the quicksave.
     - The second argument is the count which you wish to modify the score or line to be.
     - Modify will then modify your quicksave to update the score or lines to your passed in count value.
- **rank**
	- This command calls the 'rank' progam.
	- You can choose to pass in either 'score' or 'lines' as the first argument, this is the criteria by which rank will list quicksaves by.
	- The second argument is the number of ranked quicksaves you would like to see printed.

- **check**
	- This command calls the 'check' program with the current quicksave to verify if it will pass legitimacy checks.
	- Check takes no arguments.

- **recover**
	- This command calls the 'recover' program and returns a list of recovered quicksaves from the disk image file path specified by the passed in argument.
	- This command only takes in 1 argument which is the path to the disk image file you wish to recover quicksaves from.

- **exit**
	- This command calls the 'exit' program with no additonal arguments, causing the program to exit.

- **switch**
	- This command switches the current quicksave to the new quicksave found at the path you enter as the argument.

- **help**
	- The 'help' command provides information on the use for other commands
	- Enter the desired command as the argument.
	- Some possible commands are 'exit', 'check', 'modify', etc.

- **info**
	- This command provides information on the current quicksave, including the path to the quicksave, as well as its number of scores and lines.
	- To use this command, simply enter the 'info' command with no arguments.

- **undo**
	- This command reverts the last modify command executed.
	- For example, if score was previously 5, and you called modify score 100, then called undo, the score would be reverted from 100 back to 5.
	- undo does not take any arguments.
## Supported Features:

 - **Short commands**
	 - Short commands allow commands to be abbreviated to the minimum number of distinguishable characters
	 - Try typing e and pressing enter, this will run the 'exit' command

- **Quick rank**
	- The quick-rank feature allows the 'rank' feature to be called with only one argument, either 'score' or 'lines' to indicate the measure to rank on
	- Doing so will print out the first 10 quicksaves in the rankings list for your given argument

### Total Features
**Core Commands** - 70 pts  
**Short Commands** - 5 pts  
**Quick-Rank** - 5pts  
**switch** - 5 pts  
**help** - 7 pts  
**info** - 5 pts  
**undo** - 15 pts  
  
***Total*** = *112 pts*


  

Thank you! Enjoy using our tool!

  

Alec & Bailey

~
