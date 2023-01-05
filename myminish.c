#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void print()
{
	char host[500];
	char *user=getenv("USER");;
	char current[500];
	
	getcwd(current, 500);
	
	
	if(strncmp(user,current,strlen(user))==0)
	{
		strcpy(current,current+strlen(user)-sizeof(char));
		current[0]='-';
	}
	
	gethostname(host,500);
	printf("[MySh] %s@%s:~%s$",getenv("LOGNAME"),
	host,current);
	
}
int read_line()
{
	char word[500],ex[10]={"exit"};
	char letter;
	char *word2;
	int cont=0;
	fflush(stdin);
	letter=getchar();
	while(letter!='\n'){
		word[cont]=letter;
		cont++;
		word[cont]='\0';
		letter=getchar();
	}
	
	char* arg_list[500];
	cont=0;
	
	word2=strtok(word," ");
	while(word2!=NULL)
	{
		arg_list[cont]=word2;
		word2=strtok(NULL," ");
		cont++;
	}
	arg_list[cont]=NULL;
	
	
	if(strcmp(word,ex)==0)
	{
		return 1;
	}
	else
	{	
				
		  pid_t pid, wpid;
		  int status;

		  pid = fork();
		  if (pid == 0) {
		    if (execvp(arg_list[0], arg_list) == -1) {
		      fprintf(stderr,"Error: No such file or directory.\n");
		    }
		    exit(EXIT_FAILURE);
		  } else if (pid < 0) {
		    	printf("Unable to fork");
		  } else {
		    waitpid(-1, &status, 0);
		    
		  }

		  return 0;
	}
	
}
void loop (){
	
	while(1){
		print();
		int var;
		var = read_line();
		if(var==1)
			break;
	}
	
}
int main (int argc, char* argv[])
{
	loop ();
	return 0;
}


