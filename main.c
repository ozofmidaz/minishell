#include <stdio.h>
#include "main.h"
char prompt[25] = "minishell$";
SLL *head = NULL;
char input_string[25];  
void insert_at_first(int cpid, char *cmd)
{
    SLL *new = malloc(sizeof(SLL));
    if(new == NULL)
    {
        perror("malloc");
        return;
    }
    new->pid = cpid;
    strncpy(new->command, cmd, 24);
    new->command[24] = '\0';
    new->link = head;
    head = new;
}


void delete_at_first()
{
    if(head == NULL)
        return;

    SLL *temp = head;
    head = head->link;
    free(temp);
}

int main()
{
//clear the terminal dis
system("clear");
scan_input(prompt, input_string);
return 0;    
}