#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int launch(char **args,int background);
int parse_line(char*args[],char line[]);
void read_line(char line[]);
void remove_EOL(char line[]);
int split_line(char* args[],char line[]);
void shellPrompt();
int changeDirectory(char* args[]);
int flag =0 ;
int x = 0;
int main()
{
    char** args; 
    args = (char *) malloc(10*(sizeof(char**)));
    char line[100];
    while(1)
    {
        shellPrompt();
        parse_line(args,line);
    }
    return 0;
}

void shellPrompt()
{
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));

    char host[1204] = "";
    gethostname(host, sizeof(host));
    printf("%s@%s:%s> ", getenv("LOGNAME"), host,cwd);
}

int parse_line(char*args[],char line[])
{
    read_line(line);
    if(flag ==1)
    {
        flag = 0;
        return 0;
    }
    split_line(args,line);
    return 1;
}

void read_line(char line[])
{
    char *x= fgets(line,100,stdin);
    remove_EOL(line);
}

void remove_EOL(char line[])
{
    int i, length,j=0;
    int temp[100];
    length = strlen(line);
    for(i = 0 ; i < length ; i++) {
        if(line[i] == '\n')
            line[i] = '\0' ;
    }
    for (i = 0; i < length; ++i) {
        temp[j] = line[i];
        j++;
    }
    char x = line[0];
    char *p = strtok (temp, " ");
    if(strcmp(line,"\0")==0||p == NULL)
    {
        flag =1;
    }
}

int split_line(char* args[],char line[])
{
    char ** result  = NULL;
    char *  p    = strtok (line, " ");
    int n = 0, i=0,j=0,background=0,z=0;
    while (p) {
        result = realloc (result, sizeof (char*) * ++n);
        if (result == NULL)
            exit (-1);
        result[n-1] = p;
        p = strtok (NULL, " ");
    }
    result = realloc (result, sizeof (char*) * (n+1));
    result[n] = 0;
    for (i = 0; i < (n+1); ++i) {
        args[j] = result[i];
        j++;
    }
    
    if (strcmp(args[0],"cd") == 0) changeDirectory(args);
    
    else if(strcmp(args[0],"exit")==0) {
     exit(0);
    }
    
    else{
        while (args[z] != NULL && background == 0){
            if (strcmp(args[z],"&") == 0&&args[z+1]==NULL)
            {
                args[z] = NULL;
                background = 1;
            }
        z++;   
        }  
        launch(args,background); 
    }
    free (result);
    return 1;
}   

int launch(char **args,int background)
{
	 pid_t pid, wpid;
	 if((pid=fork())==-1){
		 printf("Child process could not be created\n");
		 return 0;
	 }
     if(pid==0){
         x = execvp(args[0],args);
         if (x ==-1){
			printf("Command %s not found\n",args[0]);
           exit(0);
		}
     }
     if(background == 0)
     {
            waitpid(pid,NULL,0);
     }
     else
     {
         printf("Process Created with PID:%d\n",pid);
     }
     return 1;
}

int changeDirectory(char* args[])
{
    char  gdir[1024];
    char  *dir;
    char  *to;

    if (args[1] == NULL) {
        chdir(getenv("HOME"));
        return 1;
    }

    else {
            getcwd(gdir, sizeof(gdir));
            dir = strcat(gdir, "/");
            to = strcat(dir, args[1]);
            chdir(to);
        if (chdir(to) == -1) {
            perror("Bash");
            return -1;
        }
    }
    return 0;
}