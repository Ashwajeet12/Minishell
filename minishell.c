#include "minishell.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int status; //to check exit of child
static int flag =1; // for stopped process
int ret; // to store return value of fork
int c=0; // to check if prompt is printed or not
int W=0; // to store waitpid options
int child_pid; // to store child process id
extern char external_command[200][20]; // Array to store external commands

char *input_string; // Pointer to store user input
extern char prompt[100]; // Pointer to store prompt
char ptr[100]; // Pointer to store command for stopped process
struct list // Structure for linked list to store stopped processes
{
        int pid; 
        char Command[100];
        struct list *link;
};

struct list* head = NULL; //head ptr
void extract_external_command(char arr[][20]) // Function to extract external commands from a file
{
    FILE *fptr = fopen("External_Command.txt","r"); 
    if(fptr==NULL)
    {
        perror("File\n");
    }

    int word_count=0,char_count=0,k=0; // Initialize counters for words and characters
    char buffer[20],ch; // Buffer to read characters from the file
    while((ch=fgetc(fptr))!=EOF) // Read characters until EOF
    {
         if (ch == '\r')  //
         continue;

            if(ch=='\n')
            {
                 arr[word_count][char_count] = '\0';
                 word_count++;
                 char_count=0;

            }
         else 
         { 
            arr[word_count][char_count]  = ch;
            char_count++;
         }
           
    }
    arr[word_count][char_count] = '\0';
 

}

void scan_input()
{
   while(1)
   {    
       input_string = calloc(10, 10 * sizeof(char)); //dynamically allocate memory for input_string
          signal(SIGINT,signal_handler); // Register signal handler for SIGINT
          signal(SIGTSTP,signal_handler); // Register signal handler for SIGTSTP

         if(c!=0) // check if prompt is printed or not
         {
         printf("\n\033[0;36m%s\033[0m", prompt);
         } 
         
         else   
         printf("\033[0;36m%s\033[0m", prompt); // Print the prompt
         
         fflush(stdout);
         
         if(scanf("%[^\n]", input_string) == 0) // Read input from the user
          {
                getchar(); // Clear the input buffer if no input is given
                continue; 
          }

          getchar();
          
          strcpy(ptr,input_string);
          
        if(strstr(input_string,"PS1")!=NULL)
         {   
                char *ptr = validate_PS1(input_string); // Validate PS1 command
                 if(ptr!=NULL)
                 {
                     strcpy(prompt,ptr);
                 }
                 else
                 {
                     printf("PS1: command not found\n");
                     
                 }
         }
           else 
           {   
               char buffer[20]; // Buffer to store the command
                char*cmd = get_command(input_string,buffer); // Get the command from the input string
                int command =  check_command_type(cmd); // Check the type of command (BUILTIN, EXTERNAL, NO_COMMAND)
                 
                if(command==BUILTIN)
                 {
                      execute_internal_commands(input_string);  // Execute internal commands

                 }
                 else if(command==EXTERNAL)  
                 {
                  char *argv[20];
                  c=0;
                  execute_External_commands(input_string,argv); // Execute external commands


                 }
                 else if(command == NO_COMMAND)
                 {  
                      if(strcmp(input_string,"jobs")!=0  && strcmp(input_string,"fg")!=0 && strcmp(input_string,"bg")!=0) // If the command is not found 
                             printf("%s: command not found\n",cmd);
                 }

           }

           if(strcmp(input_string,"jobs")==0) //if the command is jobs
           {   c=0;
               print_list(head); 
           }

           if(strcmp(input_string,"fg")==0) // if the command is fg
           {   
                 W=5;
                int PID =  get_last_stoped_process_id(head); // Get the last stopped process ID
                
                if(PID != -1) // check if a stopped process exists  
                {
                child_pid = 5;  
                printf("%d\n",PID);
                kill(PID,SIGCONT);
                waitpid(PID, &status,WUNTRACED);
                delete_last(&head);
               
                }
                
           }

           if(strcmp(input_string,"bg")==0)
           {    
                 c=0;
                 W=5;
                int PID =  get_last_stoped_process_id(head); // Get the last stopped process ID
                signal(SIGCHLD,signal_handler); // Register signal handler for SIGCHLD
              
                kill(PID,SIGCONT); // Continue the stopped process
                
                waitpid(PID, &status,WNOHANG); // Wait for the process to finish
                delete_last(&head);// Delete the last stopped process from the list
                child_pid = 0;
                
           }

           W=0;


   }     

}
  
 char *validate_PS1(char *input_string)
{     
      char *str = "PS1";
      int j=0,i=0;
       while(input_string[i]!='\0')
      {
           if(i<3)
           {
           if(input_string[i]!=str[j])
           {
                return NULL;
           } 
           }else  if(input_string[i]=='=' && input_string[i+1]!=' ')
                  {
                     return (input_string+i+1);    
                  }else 
                  {
                     return NULL;
                  }
           j++;
           i++;
      }

}

char *get_command(char *input_string,char*buffer) // Function to extract the command from the input string
{

      strcpy(buffer,input_string); // Copy the input string to the buffer
      int i=0; 
      while(input_string[i]!='\0') 
      {
            if(input_string[i]==' ' ) // Check for space character
            {
                  buffer[i] = '\0'; // Replace space with null character
                  return buffer; 
            }
            i++;
      }
   
      return input_string;

}

int check_command_type(char *cmd) // Function to check the type of command (BUILTIN, EXTERNAL, NO_COMMAND)
{
   
   char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
      "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
      "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL}; // Array of built-in commands

    int k=0 ; // Initialize index for external commands
      
      for(int i=0;builtins[i]!=NULL;i++) 
      {
             if(strcmp(builtins[i],cmd)==0)
             {
               
              return BUILTIN;
             }
     }

     for(int i=0;i<152;i++)
     {
   
            if(strcmp(external_command[i],cmd)==0) // check if the command is external
            {
               
                  return EXTERNAL;
            }
          
    }
    return NO_COMMAND;
}

void execute_External_commands(char *input_string,char* argv[])  // Function to execute external commands
{
      int k=0,i=0,j=0,count=0; // Initialize counters for arguments and pipes
     char *token = strtok(input_string," "); // strtok to split the input string into tokens
     while(token!=NULL)
     {
          argv[i] = token;
          token = strtok(NULL, " ");
          i++;
     }
    
     
     argv[i] = NULL; 

     while(argv[k]!=NULL) // Count the number of pipes in the command
     {
        if(strcmp(argv[k],"|")==0)
        {
            count++;
        }
        k++;
      
      }
      if(count==0)
      {
        
            int ret = fork();
            if(ret==0)
            {
              
               signal(SIGTSTP, SIG_DFL); // Reset signal handler for SIGTSTP
               signal(SIGINT, SIG_DFL); // Reset signal handler for SIGINT
              execvp(argv[0],argv); // Execute the external command
            
            }
            else if(ret>0)
            {    

                 W = child_pid =  ret;
                  waitpid(ret, &status, WUNTRACED); //
                
            }
      }
      else if(count>0) // If there are pipes in the command
      {
               n_pipe(count,argv); // Call the n_pipe function to handle piped commands
      }


}

void execute_internal_commands(char *input_string) // Function to execute internal commands
{
         
           if(strcmp(input_string,"exit")==0) 
           {
               exit(1);
           }
           else if(strstr(input_string,"cd")!=NULL) // Check if the command is 'cd'
           {
                
                  int i=0; 
                  char *ptr ;  // Pointer to store the directory path
                  char *token = strtok(input_string," "); // Split the input string by spaces
                  while(token!= NULL) //
                  {   
                         ptr = token;
                         token = strtok(NULL," "); // Get the next token

                  }

                 if(chdir(ptr) == -1) // chdir to change the directory
                  {
                    printf("-bash: cd: %s: No such file or directory\n",input_string);
                  }
                                 
          
           }
           else if(strstr(input_string,"pwd")!=NULL) // Check if the command is 'pwd'
           {

                 char buffer[100]; // Buffer to store the current working directory
                 getcwd(buffer,sizeof(buffer)); // Get the current working directory
                 printf("%s\n",buffer); // Print the current working directory
           }
           else if(strstr(input_string,"echo")!=NULL) // check the commad is echo
           {
                      
                    if(strstr(input_string,"$$")!=NULL)
                    {
                         printf("%d\n",getpid());
                    }
                    else if(strstr(input_string,"$?")!=NULL)
                    {
                          
                            printf("%d\n",WEXITSTATUS(status));
                           
                    }
                    else if(strstr(input_string,"$SHELL")!=NULL)
                    {      
                        
                           printf("%s\n",getenv("SHELL"));
                    }
                    else 
                    {
                         printf("%s\n",input_string+5);
                    }

           }

         
            
}

void n_pipe(int pipe_count,char* argv[]) // Function to handle piped commands
{
    
      int new_stdin = dup(0); // Duplicate the standard input
      int new_stdout = dup(1); // Duplicate the standard output
      int i=0,k=1; // Initialize counters for arguments and pipes
      int index[pipe_count+1]; // Array to store indices of pipe commands
      index[0] = 0; // Initialize the first index to 0
      while(argv[i]!=NULL) 
      {
             if(strcmp(argv[i],"|")==0) // Check for pipe symbol
             {
                    argv[i] = NULL; // Terminate the current command
                    index[k] = i+1; // Store the index of the next command
                    k++; // Increment the index counter

             }
             i++; // Increment the argument counter
      }
      
int prev_pipe_read = -1;  // Variable to store the previous pipe read end
int count = pipe_count; // Number of pipes in the command
for (int i = 0; i < count + 1; i++)  
{
    int fd[2]; // File descriptors for the pipe
    if (i < count) // check if there are more pipes to process
    {                           
        if (pipe(fd) == -1) // Create a pipe
        {
            perror("pipe"); // Handle pipe creation error
            exit(1);
        }
    }

    ret = fork(); // Fork a new process
    if (ret == -1) // Check for fork error
    {
        perror("fork");
        exit(1);
    }

    if(ret == 0) // Child process
     {  
        
        if (i > 0) // If this is not the first command
        {
            dup2(prev_pipe_read, STDIN_FILENO);  // Redirect standard input to the previous pipe read end
            close(prev_pipe_read); // Close the previous pipe read end
        }

        
        if (i < count) // If this is not the last command
         {
            close(fd[0]); // Close the read end of the pipe
            dup2(fd[1], STDOUT_FILENO);  // Redirect standard output to the pipe write end
            close(fd[1]); // Close the write end of the pipe
            
        }
      
        execvp(argv[index[i]], &argv[index[i]]);    // Execute the command
        perror("execvp failed"); // Handle execution error
        exit(1);
                                                 
    }
    else
     {
        if (i > 0) // If this is not the first command
        {
            close(prev_pipe_read); // Close the previous pipe read end
        }
        
        if (i < count) // If this is not the last command
        {
            prev_pipe_read = fd[0]; // Store the read end of the current pipe
            close(fd[1]);  // Close the write end of the pipe
        }
     }
}

for (int i = 0; i < count + 1; i++) // Wait for all child processes to finish
 {
    wait(NULL); // Wait for any child process to finish
 }
    

}

void signal_handler(int signum) // Signal handler function to handle signals like SIGINT, SIGTSTP, and SIGCHLD
{
        if(signum == SIGINT) // Handle SIGINT signal
           c++; // Increment the counter to indicate that SIGINT was received

      if(signum == SIGINT && W == 0) //check if no child process is running 
      {  
      printf("\n\033[0;36m%s\033[0m",prompt); // Print the prompt again
      fflush(stdout); // Flush the output buffer
      }

      else if(signum == SIGTSTP) // Handle SIGTSTP signal
      {
           
           if(input_string[0]== '\0') // Check if the input string is empty
           {
            printf("\n\033[0;36m%s\033[0m", prompt); 
            child_pid =0; // Reset child_pid to 0
            fflush(stdout); // Flush the output buffer
           }
           else if(input_string[0]!='\0') // If the input string is not empty
           {     
                printf("\n"); 
              
                printf("\033[1;35m[%d]+\033[0m       \033[1;34mStopped\033[0m              \033[1;37m%s\033[0m\n", flag, ptr); // Print the stopped process information
                
                insert_last(&head,child_pid,ptr); // Insert the stopped process into the linked list
                flag++; // Increment the flag for the next stopped process
                fflush(stdout); 
                child_pid =0;
                free(input_string); // Free the dynamically allocated memory for input_string
           }
      }
      else if(signum == SIGCHLD) // Handle SIGCHLD signal
      {
           waitpid(W, &status,WNOHANG); // Wait for the child process to change state
      }

}

void insert_last(struct list** head_ref, int pid,char *command) // Function to insert a new node at the end of the linked list
{
    struct list* new_node = (struct list*)malloc(sizeof(struct list));
    new_node->pid = pid;
    strcpy(new_node->Command,command);
    new_node->link = NULL;

    if (*head_ref == NULL) 
    {
        *head_ref = new_node;
        return;
    }

    struct list* last = *head_ref;
    while (last->link != NULL) 
    {
        last = last->link;
    }

    last->link = new_node;
}

void print_list(struct list* node)  
{
    while (node != NULL)
     {
        printf("\033[1;35m%-8d\033[0m \033[1;34m%-12s\033[0m \033[1;37m%s\033[0m\n",node->pid, "Stopped", node->Command);
        node = node->link;
    }
}

int get_last_stoped_process_id(struct list* node)
{
      if(node == NULL)
      {
           return -1;
      }

      while (node->link != NULL)
      {
         node = node->link;
      }

     return node->pid;
         
}
int delete_last(struct list** head)
{
        if(*head == NULL)
          {
              return 0;
          }
          
          if((*head) -> link == NULL)
          {
              free(*head);
              *head = NULL;
              return 1;
          }
          
           struct list *temp = *head;
          
          while(temp -> link -> link != NULL)
          {
              temp = temp -> link;
          }
          
          free(temp -> link);
          temp -> link = NULL;
          
          return 1;
             
}