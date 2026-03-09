#include <stdio.h>
#include "main.h"
int pid = 0;
int status;

void signal_handler(int signum)
{
if(signum == SIGINT){
if(pid != 0)//if pid is 0 means no foreground process running
{
    //when no command is excecuted
    //print the command prompt again
   // printf("\n");
   // printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET, prompt);
    kill(pid, SIGINT);
}
 else
    {
        printf("\n");
        printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET, prompt);
    }
}
else if(signum == SIGTSTP)
{
    if(pid != 0)
    {     
        insert_at_first(pid, input_string);
        pid = 0;

        printf("\n");
    printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET, prompt);
    }
}
else if(signum == SIGCHLD)
{
    //clear resource of chi;ld

    waitpid(-1, &status, WNOHANG);
   //waitpid((head->pid), &status, WUNTRACED);
}

}


void scan_input(char *prompt, char *input_string)
{
    //--------------------------------------------------
    //register the signal
    signal(SIGINT, signal_handler);
signal(SIGTSTP, signal_handler);
signal(SIGCHLD, signal_handler);


    //-----------------------------------------------------
    while(1){
printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET, prompt);

/*
optional: if wanted while printing command prompt use colour code(in .h file)*/
        //printf("%s", prompt);
//read i/p from user
//if i/p is PS1-> then we need to coustomize this particular environmental varaible 
scanf(" %[^\n]", input_string);

/*
we need to read I/p
but before that the program should not get terminated
so displaying the prompt should be inside while(1)--> even though infinit minishell$ will be printed
why?->just a  buffer issue bcs of using selective scanf so add space b4  %[]*/
//after reading check if tis ps1 varaiable or not


//check i/p string 1st 4 charn are PS1=
if (strncmp(input_string, "PS1=", 4) == 0)
{
    //after = we dont need spaces, so check
    if(input_string[4] != ' ')
    {
        //ps1 varaiable accessed properly
        //if done change prompt name
        //PS1=hello,after the =  only new prompt starts
        strcpy(prompt, input_string+4);
        //from = till null char, will vbe copied
    }
    else{
        //not accessed properly
        printf("ERROR____command not found\n");
    }
}    /*
    
if its not ps1 it should be internal or external or special variables
 so validate that
*/
char *command = get_command(input_string);//return value of get command is collected here
// check type of command, check what type of command-> return command from get command
int ret_command = check_command_type(command);

//printf("TYPE = %d\n", ret_command);
//---------------------------------------------//
//-----3 possiblities-----//return num there fore we return int check_command_type
//---------------------------------------------//
//BUILT_IN 1
if(ret_command == BUILTIN)
{
execute_internal_commands(input_string);//expects input_string//defenition @ commands.c
}




//---------------------------------------------//

//external 2
else if(ret_command == EXTERNAL)
{
//create child, take pid as global varaiable
pid = fork();

if(pid == 0)//child
{
        //---------------------//

    signal(SIGINT, SIG_DFL);//SIG_DFL give defaul property of the signal
    //defgault propertyy is given back only to the child process
      

    /*---ok without signal(SIGINT, SIG_DFL); child would think (pid == 0)
when its created just print minishell$
ninstead of running the command*/
    
    //---------------------//



        //write logic to execute the external command
        //1; exc the command
        //input_string = ls - l or wc -c or ls | wc

        //1: implement the logic to convert 1d array(input_string ) 
        //   to 2d array which is array of pointer
    char *argv[50];
    int argc = 0;
char temp[100];
strcpy(temp, input_string);
    argv[argc] = strtok(temp, " ");
    while(argv[argc] != NULL)
    {
    argc++;
    argv[argc] = strtok(NULL, " ");
    }

        //2: check if it has a pipe or not

int cmd_pos[50];
int cmd_count = 0;
int pipe_found = 0;
cmd_pos[cmd_count++] = 0;
for(int i = 0; i < argc; i++)
        {
            if(strcmp(argv[i], "|") == 0)
            {
                pipe_found = 1;
                argv[i] = NULL;                
                cmd_pos[cmd_count++] = i + 1;  
            }
        }
//3.1 if pipe is present -=> implement n_pipe_assignment
if(pipe_found)
{
int fd[2];
 pid_t cpid;

for(int i = 0; i < cmd_count; i++)
{
if(i < cmd_count - 1)
pipe(fd);
cpid = fork();
if(cpid == 0)
{
if(i < cmd_count - 1)
 {
close(fd[0]);
dup2(fd[1], STDOUT_FILENO);
 close(fd[1]);
}
if (execvp(argv[cmd_pos[i]], &argv[cmd_pos[i]]) == -1)
{
    perror("execvp");
    exit(1);
}
  }
else
{
if(i < cmd_count - 1)
{
 close(fd[1]);
dup2(fd[0], STDIN_FILENO);
close(fd[0]);
}
}
}

 for(int i = 0; i < cmd_count; i++)
wait(NULL);

exit(0);
}
//3.2 not present, if not present use execvp directly
else
{
 if (execvp(argv[0], argv) == -1)
    {
        perror("execvp");
        exit(1);
    }
 }

//argv of index AND address,
//argv is 2d array but string_input is collected as single array
}
else if(pid > 0)
{
//parent, it waits till child comp the task
waitpid(pid, &status, WUNTRACED);
 pid = 0; // dont want to block the parent process
}
}

//--------------------------------------------------------//





//special varaiables 3(no command)
else if(ret_command == NO_COMMAND)
{
       if(strcmp(input_string, "echo $?") == 0){
//check input string is echo $ ? ->     check the exit status_ how to collect if we used wait pid in parent 
//waitpid(pid, &status,)
//&status collects exit status of process
//wexitstatus->exit status of the child
if (WIFEXITED(status)){
 printf("%d\n", WEXITSTATUS(status));}
       }
         else if(strcmp(input_string, "echo $$") == 0)
         {  
//check the input_string echo $$
//->print pid of minishell
printf("%d\n", getpid());//to get current process pid
         }
             else if(strcmp(input_string, "echo $SHELL") == 0){
//check input_string is echo $shell
//->print exec path of minishell
printf("%s\n", getenv("SHELL"));//->it will fetch the value of environmental varaibles 
             }
            }
//---------------------------------------------//
if (strcmp(command, "jobs") == 0){
    //stopped info stored in sll
    //print_list op, print sll until reaches null 
    //if head -> null:  command prompt going to be printed head
    // if not equal to null then only we perform print list op
    if(head == NULL)
       {
     printf("No stopped jobs\n");   
       }
else{
    SLL *temp = head;

    while(temp != NULL)
    {
        printf("Stopped\t%s\n", temp->command);
        temp = temp->link;
    }
    }
}
else if(strcmp(command, "fg") == 0)
{
    if(head == NULL)
    {
        printf("fg: current: no such job\n");
    }
    else
    {
       printf("%s\n", head->command);

pid = head->pid;

kill(head->pid,SIGCONT);

waitpid(head->pid,&status,WUNTRACED);

delete_at_first();

pid = 0;
    }
}
else if (strcmp(command, "bg") == 0){
    if(head!=NULL)
{
printf("%s &\n",head->command);
kill(head->pid,SIGCONT);
}
        //terminated child generetes sigchld->then control gos to signal_handler

}

}
}