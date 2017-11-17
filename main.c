#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
void launch(char **args,int background);
    int no_reprint_prmpt = 0;
void remove_EOL(char line[])
{
    int i, lenght ;
    lenght = strlen(line);
    for(i = 0 ; i < lenght ; i++) {
        if(line[i] == '\n')
            line[i] = '\0' ;
    }
}
void read_line(char line[])
{
    char* ret = fgets(line,100,stdin);

    remove_EOL(line);

    //printf("test");
}
int XXXXX(char* args[],char line[])
{
    char ** res  = NULL;
    char *  p    = strtok (line, " ");
    int n_spaces = 0, i=0,j=0,background=0,z=0;
    while (p) {
        res = realloc (res, sizeof (char*) * ++n_spaces);

        if (res == NULL)
            exit (-1);
        res[n_spaces-1] = p;
        p = strtok (NULL, " ");
    }
    res = realloc (res, sizeof (char*) * (n_spaces+1));
    res[n_spaces] = 0;
    for (i = 0; i < (n_spaces+1); ++i) {
        args[j] = res[i];
        j++;
    }
    if (strcmp(args[0],"cd") == 0) changeDirectory(args);
        
    else if(strcmp(args[0],"clear")==0) {
        system("clear");
    }
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
        
//        printf("%d",background);
        launch(args,background); 
    }
//    for(i=0; i<j+1; ++i) {
//        printf ("res[%d] = %s\n", i, args[i]);
//    }
    free (res);
    return 1;
}   

void launch(char **args,int background)
{
     int err = -1;
	 pid_t pid, wpid;
	 if((pid=fork())==-1){
		 printf("Child process could not be created\n");
		 return;
	 }
     if(pid==0){
         if (execvp(args[0],args)==err){
			printf("Command %s not found\n",args[0]);
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
}
int parse_line(char*args[],char line[])
{
	if(line == NULL)
	{
		free(line);
		return 0;
	}
    read_line(line);
    XXXXX(args,line);
    return 1;
}
void shellPrompt()
{
    // We print the prompt in the form "<user>@<host> <cwd> >"
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));

    char hostn[1204] = "";
    gethostname(hostn, sizeof(hostn));
    printf("%s@%s:%s> ", getenv("LOGNAME"), hostn,cwd);
}
int changeDirectory(char* args[])
{
    char  gdir[1024];
    char  *dir;
    char  *to;
    // If we write no path (only 'cd'), then go to the home directory
    if (args[1] == NULL) {
        chdir(getenv("HOME"));
        return 1;
    }
    // Else we change the directory to the one specified by the
    // argument, if possible
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
int main()
{
    char** args;
    args = (char *) malloc(10*(sizeof(char**)));
    char line[100];
    pid_t pid, wpid;
    int status; 

    int flag = 1;
    while(flag!=0)
    {
        shellPrompt();
        parse_line(args,line);
    }
    return 0;
}
