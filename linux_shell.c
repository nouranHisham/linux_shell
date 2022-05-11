#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

void handleLog(int sig);

int main()
{

     signal(SIGCHLD, handleLog);

     //character arrays to hold the commands and the delimmeter
     char input_array[256];
     char delim[2]=" ";
     char *token;
     char *input;
     char *args[16];
     int flag;

     //counters
     int i;
     int counter;

     //do-while loop to take commands until exit is entered
     do{
     i=0;
     counter=0;
     memset(args, 0, 16);
     printf("shell >> ");
     fgets(input_array, sizeof(input_array), stdin);

     if ((strlen(input_array) > 0) && (input_array[strlen(input_array)-1] == '\n'))
     input_array[strlen(input_array)-1] = '\0';

     //to allocate the command
     input = (char*)malloc(100);
     strcpy(input,input_array);

     //to parse through the command
     token = strtok(input, delim);

     while(token != NULL)
     {
         //printf("%s \n", token);

         args[i] = token;
         i++;

         counter++;

         token = strtok(NULL, delim);
     }

     //printf("check %d\n", strcmp("&",args[i-1]));

     if(strcmp("&",args[i-1]) == 0)
     {
     args[i-1] = NULL;
     args[i] = "&";
     }
     else
     {
     args[i] = NULL;
     }

     pid_t pid = fork();

     if(pid == -1)
     printf("failed to create a child\n");
     else if(pid == 0)
     {
     if((execvp(args[0],args) < 0) && (strcmp("exit",input) != 0))
     {
     perror(input);
     exit(EXIT_FAILURE);
     }
     }
     else
     {
     if(args[i] == NULL)
     waitpid(pid, NULL, 0);
     }
     }
     while(strcmp("exit",input_array) != 0);

     if(strcmp("exit",input) == 0)
     {
     free(input);
     exit(0);
     }


     //testing (remove later)
     //for(i=0;i<counter;i++)
     //{
     //printf("check ");
     //printf("%s\n",args[i]);
     //}

     //to exit the program and terminate the shell
     /*if(strcmp("exit",input) == 0)
     {
     //printf("shell terminated\n");
     free(input);
     exit(0);
     }*/

     return 0;
}

void handleLog(int sig)
{
FILE *filePointer;
filePointer = fopen("log.txt","a");
if(filePointer == NULL) perror("Error opening file");
else fprintf(filePointer, "[LOG] child process terminated.\n");
fclose(filePointer);
}
