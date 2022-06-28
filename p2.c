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
#define BYTES 512

static FILE *diskfile;
static int nsetores=0;

struct fs_setor{
	char name[1024];
  	int address;
	int pointers[SETORES];
};

int disk_init( const char *filename, int n )
{
  	int newCreation = 0;
	diskfile = fopen(filename,"r+");
	if(!diskfile) {
    	diskfile = fopen(filename,"w+");
    	newCreation = 1;
  	};
	if(!diskfile) return 0;

	ftruncate(fileno(diskfile),n*BYTES);

	nsetores = n;
  	if(newCreation == 0){
		return 1;
  	}
  	return 2;
}

static void sanity_check( int num)
{
	if(num<0) {
		printf("ERRO: numero do setor é negativo!\n");
		abort();
	}

	if(num>=nsetores) {
		printf("ERRO: numero do setor é muito grande!\n");
		abort();
	}
}


void disk_write( int num, struct fs_setor data )
{
	sanity_check(num);

	fseek(diskfile,num*BYTES,SEEK_SET);

	if(num >= 9) {
		if(fwrite(&data,sizeof(struct fs_setor),1,diskfile)==1) {
			return;
		} else {
			printf("ERRO: não foi possivel acessar o disco: %s\n",strerror(errno));
			abort();
		}
	} else {
		printf("Erro: segmento ocupado pelo sistema\n");
		return;
	}
}

void disk_close()
{
	if(diskfile) {
		fclose(diskfile);
		diskfile = 0;
	}
}

struct fs_setor Setor(char* name, int espaco)
{
  struct fs_setor setor;
  int i = 0;
  strcpy(setor.name, name);
  for(i = 0; i< SETORES -1; i++){
    setor.pointers[i] = 0;
  }
  setor.address = espaco;
  return setor;
}


int fs_create_home(){
	struct fs_setor setor;
	setor = Setor("Home", 9);
	disk_write(9, setor);
	return 1;
}

int main( int argc, char *argv[] ){

	char nome_disco[]="Disco";

	char line[1024];
	char cmd[1024];
	char arg1[1024];
	int args;
  	int diskCreated = disk_init(nome_disco, SETORES);
  	
	if(diskCreated == 0) 
	{
		printf("Não foi possivel inicializar %s: %s\n",argv[1],strerror(errno));
		return 1;
	} 
	else if (diskCreated == 2) 
	{
    		fs_create_home();
  	}	   
  	printf("      ░░░░░░░░░░░░░░░░░░░░░░\n");
  	printf("      ░░┌──┐░░░░░░░░░░┌──┐░░\n      ░╔╡▐▐╞╝░░┌──┐░░╔╡▐▐╞╝░\n      ░░└╥╥┘░░╚╡▌▌╞╗░░└╥╥┘░░\n      ░░░╚╚░░░░└╥╥┘░░░░╚╚░░░\n      ░░░░░░░░░░╝╝░░░░░░░░░░");
  					 
	printf("\n      ░░░░░░░░░░░░░░░░░░░░░░");
	printf("\n -----------------------------------------------");
	printf("\n|Bem vindo ao %s com %d setores de %d bytes|\n",nome_disco,SETORES,BYTES);
	printf(" -----------------------------------------------\n");
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
	}

	printf("O %s agradece! Volte sempre!.\n",nome_disco);
	disk_close();
	
	return 0;
}

