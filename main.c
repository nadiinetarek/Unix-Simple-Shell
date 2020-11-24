#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>


int inp_redir_flag;
int otp_redir_flag;
int pipe_flag;
char* inp_file = NULL;
char* oup_file =NULL;
int foregroundFlag=0;

FILE *f;

void remove_endOfLine(char line[])
{
    int i=0;
    while(line[i] != '\n')
        i++;

    line[i]='\0';

}


int process_line(char* args[],char line[])
{
int i=0;
args[i]= strtok(line," ");
if(args[i] == NULL)
{
    return 1;
}
while(args[i] != NULL)
{  i++;
    args[i]=strtok(NULL," ");
}
    return 1 ;
}


int pipe_redir_check(char* temp)
{
    int i=0;
    while(temp[i] != NULL) {
        if (strcmp(temp[i], ">") == 0) {
            otp_redir_flag=1;
            oup_file= temp[i+1];
    return i;
        }
        if (strcmp(temp[i], "|") == 0)
        {   pipe_flag=1;
            return i;
        }
        if(strcmp(temp[i],"<")==0)
        {
            inp_redir_flag=1;
            inp_file= temp[i+1];
            return i;
        }
        i++;
    }
    return i;
}



void signal_handler(int signal)
{
    if (signal == SIGCHLD)
    {

        f=fopen("logfile.log","a");
        fputs("child terminated, \n", f);
        printf("child terminated \n");
        fclose(f);

    }
}
void read_line(char line[])
{
    char *x = fgets(line, 100, stdin);
    printf("%s", line);
    remove_endOfLine(line);
    if (strcmp(line, "exit") == 0 || x == NULL)
    { exit(0); }
    int i=0;
    while ( line[i] != '\0') {
        if (line[i]=='&') {
            foregroundFlag =1;
            line[i]='\0';
        }
        i++;
    }
}


int read_process_line(char* args[],char line[]) {
    read_line(line);
    process_line(args, line);
    return 1;}




int main() {

    signal(SIGCHLD,signal_handler);

   char line [100];
   char* pt [10];

   
  while(read_process_line(pt,line)) {
      pid_t pid = fork();   //create a copy from a process with diff id

      if (foregroundFlag == 1 && pid != 0)
          execvp(pt[0], pt);

      else if (pid == 0) {
          // if child;
          if (inp_redir_flag == 1 && inp_file != NULL)
              dup2(open(inp_file, O_RDWR | O_CREAT, 0777), 0);

          if (otp_redir_flag == 1 && oup_file != NULL)
              dup2(open(oup_file, O_RDWR | O_CREAT, 0777), 1);


          execvp(pt[0], pt);  //take path and name  of a prog - activate and print content of prog
      } else {
          // if parent;

          wait(pid);
         /* inp_redir_flag = 0;
          otp_redir_flag = 0;
          pipe_flag = 0;
          inp_file = NULL;
          oup_file = NULL;*/
      }
  }

  foregroundFlag = 0;
    return 0;
      }




