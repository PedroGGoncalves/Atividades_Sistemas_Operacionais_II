#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
void sig_handler(int sinal)
{
	fflush(stdin);
	fflush(stdout);
}
void print()/* 1.Exibe um prompt e aguarda entrada do usuário*/
{
	char host[500],*user=getenv("HOME"),current[500];
	
	getcwd(current, 500);
	
	if(strncmp(user,current,strlen(user))==0)
	{
		strcpy(current,current+strlen(user)-sizeof(char));
		current[0]='~';
	}
	
	gethostname(host,500);
	/*6.O prompt deve ter o formato: [MySh] nome-de-usuario @hospedeiro:diretorio-atual$*/
		/*7.No prompt, o caminho do diretório home do usuário deve ser substituído por ~, 
		caso este faça parte do caminho atual.*/
	printf("[MySh] %s@%s:%s$",getenv("LOGNAME"),
	host,current);
}
int read_line()
{
	char word[500],ex[10]={"exit"},letter,*word2,*listaAux[500],*arg_list[500], *arg_pipe[500];
	int cont=0,fds[2],fd_in = 0,k=0,i=0, status,cont_pipe=0,cont2=0,cont3=1,vet[500],j=0,status2;;
 	pid_t pid_pipe,pid, wpid;
 	
	fflush(stdin);
	letter=getchar();
	
	while(letter!='\n'){
		word[cont]=letter;
		cont++;
		word[cont]='\0';
		letter=getchar();
		/*11.Ctrl+D deve sair do processador de comandos, tal qual se o comando exit
		tivesse sido utilizado*/
		if(feof(stdin))
		{
			printf("^D\n");
			return 1;
		}
	}
	
	cont=0;
	
	word2=strtok(word," ");
	while(word2!=NULL)/*Irá formar arg_list*/
	{
		arg_list[cont]=word2;
		if(strcmp(arg_list[cont], "|")==0)/*Irá contar qtos | tem*/
		{
			cont_pipe++;
			j++;
			cont3=0;
		}
		else{ /*Irá contar qta palavras/comandos tem*/
			arg_pipe[cont2]=arg_list[cont];
			cont2++;
		}
		word2=strtok(NULL," ");
		cont++;
		vet[j]=cont3;
		cont3++;
		
	}
	arg_list[cont]=NULL;
	
	/*2.O comando exit deve sair do processador de comandos*/
	if(strcmp(word,ex)==0)
	{
		return 1;
	}
	/*8.Implemente o comando cd para mudar diretórios (quando recebe argumento) ou 
									voltar ao diretório home do usuário (sem argumentos ou com o argumento ~)*/
	else if(strcmp("cd",word)==0)
	{
		if(arg_list[1]==NULL || strcmp(arg_list[1],"~")==0){
			chdir(getenv("HOME"));
		}
		else{
		/*9.Mostre uma mensagem de erro adequada se cd falhar*/
			if(chdir(arg_list[1])!=0){
				fprintf(stderr,"Error: No such file or directory.\n");
			}
		}
		return 0;
	}
	/*) 12.O processador de comandos deve permitir o uso de pipes. O símbolo | indica a 
			separação entre cada programa, conectando a saída padrão do programa à esquerda 
			om a entrada padrão do programa à direita através de um pipe*/
	else if (cont_pipe>0){

 i = 0;
 j=0;
			//Continuar criando pipes enquanto i < cont2
			while (i < cont2)
			{
				if (i/2 < cont_pipe)
				{
					pipe(fds); //Pipe
				}
				//Fork
				pid_pipe = fork();

				//Filho
				if (pid_pipe == 0)
				{
					dup2(fd_in, 0); 
					if (arg_pipe[i + 1] != NULL)
					{
						dup2(fds[1], 1);
					}

					close(fds[0]);

					listaAux[k] = arg_pipe[i];
					listaAux[k + 1] = arg_pipe[i + 1];
					
					
					if (execvp(listaAux[k], listaAux) == -1)
					{
							printf("Error: No such file or directory.\n");
					}
					exit(EXIT_FAILURE);
				}
				else if (pid_pipe < 0)
				{
				
					printf("Erro no fork!\n");
				}
				else
				{
				
					
					waitpid(pid_pipe, NULL, 0);
					close(fds[1]);

					
					fd_in = fds[0];

					i = i + vet[j];
					j++;
				}
				
			}

  return 0;
	}
	else
	{	
				/*3.Quando a entrada do usuário é o nome de um programa no path ou o caminho
		completo ou relativo de um programa, o mesmo deve ser executado*/
		/*4.Quando a entrada do usuário não é um programa ou comando válido, deve ser 
		mostrada uma mensagem de erro adequada*/	
		/*5.Os argumentos digitados na linha de comando devem ser passados ao programa 
		que será executado.*/

		  pid = fork();
		  if (pid == 0) {
		    if (execvp(arg_list[0], arg_list) == -1) {
		      fprintf(stderr,"Error: No such file or directory.\n");
		    }
		    exit(EXIT_FAILURE);
		  } else if (pid < 0) {
		    	printf("Unable to fork");
		  } else {
		    waitpid(-1, &status2, 0);
		    
		  }

		  return 0;
	}
	
}
void loop (){
	
	while(1){
		print();
		int var;
		/*10.Ctrl+Z e Ctrl+C não devem causar a parada ou interrupção do processador de 
			comandos*/
		signal(SIGINT,sig_handler);
		signal(SIGTSTP,sig_handler);
		var = read_line();/*Recebe retorno da read_line*/
		if(var==1) /*Sair do processador*/
			break;
	}
	
}
int main (int argc, char* argv[])
{
	loop ();
	return 0;
}


