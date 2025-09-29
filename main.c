/*
Name : Ashwajeet Kshirsagar
Batch : 24038
Porject Name : Minishell
*/





#include "minishell.h"
#include<stdio.h>
char external_command[200][20]; // Array to store external commands
char prompt[100] = "Minishell$:"; // Default prompt






int main()
{
    
    
     system("clear"); // Clear the terminal screen
     extract_external_command(external_command); // Load external commands from file
     
     scan_input();// Start scanning input from the user
     
}