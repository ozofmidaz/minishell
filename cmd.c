#include "main.h"

//how to validate bulid in commands
char *builtins[] = {"printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};

void execute_internal_commands(char *input_string)
{
    char *command = get_command(input_string);

    //have 3 possibilities
//1; command is exit
//if its exit : directly call exit function with arg 0-> exit(0); ->this exits entire terminal
    if (strcmp(command, "exit") == 0)
    {
        exit(0);
    }

    
//2. command is pwd
//refer the function getcwd();
    else if (strcmp(command, "pwd") == 0)
    {
        char buffer[50];
        getcwd(buffer, 50);
        printf("%s\n", buffer);
    }

    //3. command is cd
//refer the function chdir();-> change the current working directory of the process
//personally implement the chdir
   else if (strcmp(command, "cd") == 0)
{
    char *path = strtok(input_string, " ");
    path = strtok(NULL, " ");

    if (path == NULL)
        chdir(getenv("HOME"));
    else
        chdir(path);
}
}



char *get_command(char *input_string)
{
static char command[25] = {'\0'};//copy 1st command and store
//without static memory for command will be allocated in command array stack , stack cant be returend 
//once function defenition completed stack will be deleted, now its stored in (data segment)
  memset(command, 0, sizeof(command)); 
int i = 0;
//what this do
//if taking i/p string 
/*
1: for just : ls no error
2;ls -l
3: ls -l | wc -c
commands will be present in 1st word itself so fetch 1st word->how?
*/
while (input_string[i] != ' ' && input_string[i] != '\0')//so only 1st command runs
{
command[i] = input_string[i];
i++;
}
command[i] = '\0';
return command;

}//still we dont know what type of command we use
//in scan_input after get_command check type of command
//-----------------------------------------------------------------//
int check_command_type(char *command)

{//printf("COMMAND = %s\n", command);
    int i = 0;
//how to validate bulid in commands, if command matches with any of *buildins return buildin
while(builtins[i] != NULL)//if while is terminated its not a build in 
{
    if(strcmp(builtins[i], command) == 0)
    return BUILTIN;
    i++;
}
if (strcmp(command, "echo") == 0)
    return NO_COMMAND;
//take commands from the ext_cmd.txt and compare
//1. open external_commands.txt in read only mode
//printf("COMMAND = [%s]\n", command);
   FILE *fp = fopen("/mnt/e/Linux_internals/day2_minishell/ext_cmd.txt", "r");
    if (fp == NULL)
        return NO_COMMAND;

    char file_cmd[100];
//2. read file contents line by line

    while (fgets(file_cmd, sizeof(file_cmd), fp) != NULL)
    {
        file_cmd[strcspn(file_cmd, "\r\n")] = '\0';
      //  printf("FILE = [%s]\n", file_cmd);
  //printf("FILE_CMD = %s\n", file_cmd);
        //3. compare our command with the file content|
        if (strcmp(file_cmd, command) == 0)
        {
            fclose(fp);
            //4. if it matches return EXTERNAL
            return EXTERNAL;   
        }
//5. if dosent match continue with next line of the file (we should repeat this until end of file)
    }
        fclose(fp);
 
//at last should return NO_COMMANDS(once we reached end of file it means its not a buildin command and external command)
    return NO_COMMAND;


}