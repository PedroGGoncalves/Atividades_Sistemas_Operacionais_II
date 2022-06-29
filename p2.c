/*Lucas Bastelli Spagnol
Pedro Gabriel Gonçalves
*/
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define SETORES 256
#define BYTES 10000

#define GRAY "\x1B[0m"
#define BOLD "\033[1m"
#define NONE "\033[0m"
#define GREEN "\x1B[32m"
#define RED  "\x1b[31m"
#define CYAN "\e[0;36m"
#define RESET  "\e[0m"
#define BLACK  "\e[40m"


static FILE *diskfile;
static int nsetor=0;

struct setor {
	char nome[1024];
  	int endereco;
	int ponteiros[SETORES];
};

int init_disco( const char *filename, int n )
{
  	int cond = 0;
	diskfile = fopen(filename,"r+");
	if(!diskfile) 
	{
	    diskfile = fopen(filename,"w+");
	    cond = 1;
  	};
	if(!diskfile) return 0;

	ftruncate(fileno(diskfile),n*BYTES);

	nsetor = n;
  	if(cond == 0)
  		return 1;
  		
  	return 2;
}

static void check( int n )
{
	if(n<0) {
		printf("ERRO: numero do setor (%d) Ã© negativo!\n",n);
		abort();
	}

	if(n>=nsetor) {
		printf("ERRO: numero do setor (%d) Ã© muito grande!\n",n);
		abort();
	}
}

void read_disco( int n, struct setor *data )
{
	check(n);
  	if(n>= 9){
		fseek(diskfile,n*BYTES,SEEK_SET);
		if(fread(data,sizeof(struct setor),1,diskfile)==1) {
			return;
		}
		else {
			printf("ERRO: nÃ£o foi possivel acessar o disco: %s\n",strerror(errno));
			abort();
		}
	} else {
		printf("Erro: segmento ocupado pelo sistema\n");
		return;
	}
}

void write_disco( int n, struct setor data )
{
	check(n);

	fseek(diskfile,n*BYTES,SEEK_SET);

	if(n>= 9) {
		if(fwrite(&data,sizeof(struct setor),1,diskfile)==1) 
			return;
		 else {
			printf("ERRO: nÃ£o foi possivel acessar o disco: %s\n",strerror(errno));
			abort();
		}
	} else {
		printf("Erro: segmento ocupado pelo sistema\n");
		return;
	}
}

void close_disco()
{
	if(diskfile) {
		fclose(diskfile);
		diskfile = 0;
	}
}

struct setor func_setor(char* nome, int espaco){
  struct setor setorr;
  int i = 0;
  strcpy(setorr.nome, nome);
  for(i = 0; i< SETORES -1; i++)
    setorr.ponteiros[i] = 0;
  
  setorr.endereco = espaco;
  return setorr;
}

int create_home(){
	struct setor setorr;
	setorr = func_setor("Home", 9);
	write_disco(9, setorr);
	return 1;
}

int criad(char* path)
{
	struct setor setorr;
  	struct setor setorrr;
  	int aux = 0;
	read_disco(9, &setorr);

  	char delim[] = "\\";

	char *ptr = strtok(path, delim);
	while(ptr != NULL && aux < 8){
		int i = 0;
		int found = 0;
		for(i = 0; i < SETORES -1 && found == 0; i++){
			fflush(stdout);
			if(setorr.ponteiros[i] != 0){
				read_disco(setorr.ponteiros[i], &setorrr);
				if(!strcmp(ptr, setorrr.nome)) {
					printf("Dir %s existe\n", setorrr.nome);
					found = 1;
					read_disco(setorr.ponteiros[i], &setorr);
				}
			}
		}
		if(found == 0){
      		int done = 0;
			for(i = 9; i < SETORES-1 && done == 0; i++){
			
				read_disco(i, &setorrr);
				if(setorrr.nome[0] == 0){
					setorrr = func_setor(ptr, i);
					write_disco(i, setorrr);
          
          			int auxiliar = 0;
          
					while(done == 0 && auxiliar < 255) {
						if(setorr.ponteiros[auxiliar] == 0){
						setorr.ponteiros[auxiliar] = i;
						fflush(stdout);
						done = 1;
						}
						auxiliar ++;
					}
					fflush(stdout);
					write_disco(setorr.endereco, setorr);
				}
			}
			printf("Criado na pos %d\n", i-1);
			read_disco(i-1, &setorr);
		}
		ptr = strtok(NULL, delim);
		aux++;
		fflush(stdout);
	}
	if(aux >= 8)
    		printf("O tamanho maximo Ã© 8\n");

	fflush(stdout);
	return 1;
}

int mapa(){
	int i = 0;
	struct setor setorr;
	
	for(i=0; i< SETORES-1; i++){
		if(i < 10)
			printf(BLACK" "RESET);
		else{
			read_disco(i,&setorr);
			if(setorr.nome[0] == 0)
				printf(CYAN".");
			else 
				printf(RED".");
	
		}
  	}
	printf("\n"RESET);
	fflush(stdout);
	return 1;
}


int main( int argc, char *argv[] ){
	char line[1024];
	char cmd[1024];
	char arg1[1024];
	int args;
	char nome_disco[1024]="Disk";
  	int diskCreated = init_disco(nome_disco, SETORES);
  	
	if(diskCreated == 0) {
		printf("NÃ£o foi possivel inicializar %s: %s\n",nome_disco,strerror(errno));
		return 1;
	} else if (diskCreated == 2) {
    	create_home();
  	}
printf(GREEN"      â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘\n");
  	printf("      â–‘â–‘â”Œâ”€â”€â”â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â”Œâ”€â”€â”â–‘â–‘\n      â–‘â•”â•¡â–â–â•žâ•â–‘â–‘â”Œâ”€â”€â”â–‘â–‘â•”â•¡â–â–â•žâ•â–‘\n      â–‘â–‘â””â•¥â•¥â”˜â–‘â–‘â•šâ•¡â–Œâ–Œâ•žâ•—â–‘â–‘â””â•¥â•¥â”˜â–‘â–‘\n      â–‘â–‘â–‘â•šâ•šâ–‘â–‘â–‘â–‘â””â•¥â•¥â”˜â–‘â–‘â–‘â–‘â•šâ•šâ–‘â–‘â–‘\n      â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â•â•â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘");
  					 
	printf("\n      â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘â–‘");
	printf(RED"\n -----------------------------------------------");
	printf("\n|Bem vindo ao %s com %d setores de 512 bytes|\n",nome_disco,SETORES);
	printf(" -----------------------------------------------\n"GRAY);
	while(1) {
		printf("# ");
		fflush(stdout);

		if(!fgets(line,sizeof(line),stdin)) break;

		if(line[0]=='\n') continue;
		line[strlen(line)-1] = 0;

		args = sscanf(line,"%s %s",cmd,arg1);
    		fflush(stdout);
		if(args==0) continue;
		if(!strcmp(cmd,"sair")) {
				break;
			} 
		else if(!strcmp(cmd,"ajuda")) {
				printf(BOLD"Comandos:\n");
			
				printf("criad caminho\\nome_do_diretorio		->cria diretorio\n");
				printf("criaa caminho\\nome_do_arquivo tamanho	->cria arquivo\n");
				printf("removed caminho\\nome_do_diretorio	->deleta diretorio\n");
				printf("removea caminho\\nome_do_arquivo		->deleta arquivo\n");
				printf("verd caminho				->mostra diretorio\n");
				printf("verset caminho\\nome_do_arquivo		->mostra granulos ocupados pelo arquivo\n");
				printf("mapa					->mostra tabela de granulos\n");
				printf("arvore					->arvore de diretorios\n");
				printf("ajuda					->listar comandos\n");		
				printf("sair					->sair do simulador\n"NONE);
			}
		else if(!strcmp(cmd, "mapa")){
		      	mapa();
		      	
		    	} 
		else if(!strcmp(cmd,"criad")) {
			if(args==2) {
				criad(arg1);
			} else {
				printf("criad caminho\\nome_do_diretorio\n");
			}
		} 
		
		
	}

	printf("O %s agradece! Volte sempre!.\n",nome_disco);
	close_disco();
	
	return 0;
}
