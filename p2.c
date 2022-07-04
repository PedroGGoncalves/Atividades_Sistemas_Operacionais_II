/*Lucas Bastelli Spagnol
Pedro Gabriel Gonçalves
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include<sys/time.h>

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
static int cont=0;
static int cont2=0;
static int tam_arq=0;

struct setor {
    char nome[1024];
    int endereco;
    int ponteiros[SETORES];
    int tam;
};

int removea(const char * filename){
    if (remove(filename) == 0) {
        printf("Arquivo deletado\n");
    } 
    else {
        printf("Arquivo não deletado\n");
    }
    return 0;
}


int init_disco(const char *filename){ //inicia disco
    int cond = 0;
    diskfile = fopen(filename,"r+");
    if(diskfile==0){
        diskfile = fopen(filename,"w+");
        cond = 1;
    }
    if(diskfile==0) 
        return 0;//Não conseguiu criar o disco

    ftruncate(fileno(diskfile),SETORES*BYTES);

    if(cond == 0)
        return 1; //ja tem o disco criado

    return 2; //conseguiu criar o disco
}

static void check( int n ){ //checar valor do setor antes de escrever
    if(n<0) {
        printf("ERRO: numero do setor negativo!\n");
        abort();
    }

    if(n>=SETORES) {
        printf("ERRO: numero do setor é grande!\n");
        abort();
    }
}

void read_disco( int n, struct setor *data ){ //ler o disco
    if(n>= 9){
        fseek(diskfile,n*BYTES,SEEK_SET);
        if(fread(data,sizeof(struct setor),1,diskfile)==1) {
            return;
        }
        else {
            printf("ERRO: Ao acessar o disco\n");
            abort();
        }
    } else {
        printf("Erro: Ocupado pelo sistema\n");
        return;
    }
}

void write_disco( int n, struct setor data ){ //escrever no disco
    check(n);

    fseek(diskfile,n*BYTES,SEEK_SET);

    if(n>= 9) {
        if(fwrite(&data,sizeof(struct setor),1,diskfile)==1)
            return;
        else {
            printf("ERRO: Ao acessar o disco\n");
            abort();
        }
    } else {
        printf("Erro: Ocupado pelo sistema\n");
        return;
    }
}

int arvore (int nivel, struct setor setorr, int op) { //percorre arv recursivamente
    int i = 0;
    struct setor setorrr;
    //op=1 arvore op2=verd
    if(op==1){
        printf("\n");
        for(i=0; i< nivel; i++){
            if(i==nivel-1)
                printf("  |_ ");
            else
                printf("   ");
        }
        printf("%s", setorr.nome);
        fflush(stdout);
    }

    for(i=0; i < SETORES -1; i++){
        if(setorr.ponteiros[i] != 0){
            read_disco(setorr.ponteiros[i], &setorrr);
            if(op==1)
                arvore(nivel+1,setorrr,op);
            else{
                printf("%s", setorrr.nome); fflush(stdout);
                if(setorrr.tam==0){
                    printf(" Subdiretorio\n");cont++;
                }
                else{
                    printf(" Arquivo - Tamanho %d\n",setorrr.tam);cont2++;tam_arq=tam_arq+setorrr.tam;
                //gettimeofday(&setorrr,NULL);
                }
            }
        }
    }
    if(op==2){ //listagem
        int total=0;
        struct setor setorrrr;

        for(i=0; i< SETORES-1; i++){
            if(i < 10)
                total++;
            else{
                read_disco(i,&setorrrr);
                if(setorrrr.nome[0] != 0)
                    total++;
            }
        }
        printf("Total Arquivos:%d\nTotal Diretorios:%d\nTotal Tamanho Arquivos:%d\nTotal Espaco Livre:%d\n",cont2,cont,tam_arq,(SETORES*512-tam_arq-total*512));

    }
    fflush(stdout);
    return 0;
}



struct setor func_setor(char* nome, int espaco, int tam){
  struct setor setorr;
  int i = 0;
  strcpy(setorr.nome, nome);
  for(i = 0; i< SETORES -1; i++)
    setorr.ponteiros[i] = 0;
 
  setorr.endereco = espaco;
  setorr.tam=tam;
  return setorr;
}



int create_raiz(){ //cria raiz
    struct setor setorr;
    setorr = func_setor("RAIZ", 9,0);
    write_disco(9, setorr);
    return 1;
}



int criad(char* path) //cria diretorio
{
    struct setor setorr;
    struct setor setorrr;
    int aux = 0,i, aux2,cond,j;
    read_disco(9, &setorr);

    char *ptr = strtok(path, "\\");

    while(ptr != NULL && aux < 8){//enquanto n for null continua (pode ter subdiretorio)
        i = 0;
        aux2 = 0;
        for(i = 0; i < SETORES -1 && aux2 == 0; i++){ //procurar diretorio
            fflush(stdout);
            if(setorr.ponteiros[i] != 0){
                read_disco(setorr.ponteiros[i], &setorrr);
                if(strcmp(ptr, setorrr.nome)==0) {//achou
                    printf("Diretorio %s\n", setorrr.nome);
                    aux2 = 1;
                    read_disco(setorr.ponteiros[i], &setorr);
                }
            }
        }
        if(aux2 == 0){ //n achou o diretorio, entao ira escreve-lo
            cond = 0;
        for(i = 9; i < SETORES-1 && cond == 0; i++){
            read_disco(i, &setorrr);
            if(setorrr.nome[0] == 0){
                setorrr = func_setor(ptr, i,0);
                write_disco(i, setorrr);        
                j = 0;                      
                while(cond == 0 && j < SETORES-1) {
                    if(setorr.ponteiros[j] == 0){
                        setorr.ponteiros[j] = i; //ponteiro diretorio
                        fflush(stdout);
                        cond = 1;
                    }
                    j ++;
                }
                fflush(stdout);
                write_disco(setorr.endereco, setorr);
            }
        }
        printf("Criado na pos %d\n", i-1);
        read_disco(i-1, &setorr);
        }
        ptr = strtok(NULL, "\\");
        aux++;
        fflush(stdout);
    }
    if(aux >= 8)
        printf("O tamanho maximo é 8\n");

    fflush(stdout);
    return 1;
}



int verd(char* path){
    struct setor setorr;
    struct setor setorrr;
    int aux = 0,i, aux2,cond,j;
    read_disco(9, &setorr);

    char *ptr = strtok(path, "\\");

    while(ptr != NULL){//enquanto n for null continua (pode ter subdiretorio)
        i = 0;
        aux2 = 0;
        for(i = 0; i < SETORES -1 && aux2 == 0; i++){//procurar diretorio
            fflush(stdout);
            if(setorr.ponteiros[i] != 0){
                read_disco(setorr.ponteiros[i], &setorrr);
            if(strcmp(ptr, setorrr.nome)==0) {//achou
                printf("Diretorio %s existe\n", setorrr.nome);
                aux2 = 1;
                read_disco(setorr.ponteiros[i], &setorr);
            }
            }
        }
        if(aux2 == 0){//n achou o diretorio
            printf("Diretorio nao existe\n");
            break;
        }
        ptr = strtok(NULL, "\\");
        fflush(stdout);
    }
    if(aux2 != 0){ //subdiretorios ou arq
        arvore(0,setorr,2);
        cont=0;cont2=0;tam_arq=0;
    }
    fflush(stdout);
    return 1;
}


int mapa(){
    int i = 0;
    struct setor setorr;

    for(i=0; i< SETORES-1; i++){
        if(i < 10)
            printf(BLACK" "RESET); //sistema
        else{
            read_disco(i,&setorr);
        if(setorr.nome[0] == 0)
            printf(CYAN".");//livre
        else
            printf(RED"."); //ocupado
        }
    }
    printf("\n"RESET);
    fflush(stdout);
    return 1;
}

int main(int argc, char *argv[]){
    char line[1024],cmd[1024],arg[1024],nome_disco[1024]="Disco";
    int args, disco;

    disco = init_disco(nome_disco); //inicia disco
    
    if(disco == 0) {
        printf("ERRO: Ao inicializar: %s\n",strerror(errno));
        return 1;
    } 
    else if (disco == 2)
        create_raiz(); //cria raiz
    /*Inicio print inicial*/
    printf(GREEN"      ░░░░░░░░░░░░░░░░░░░░░░\n      ░░┌──┐░░░░░░░░░░┌──┐░░\n      ░╔╡▐▐╞╝░░┌──┐░░╔╡▐▐╞╝░\n      ░░└╥╥┘░░╚╡▌▌╞╗░░└╥╥┘░░\n      ░░░╚╚░░░░└╥╥┘░░░░╚╚░░░\n      ░░░░░░░░░░╝╝░░░░░░░░░░\n      ░░░░░░░░░░░░░░░░░░░░░░");
    printf(RED"\n -----------------------------------------------");
    printf("\n|Bem vindo ao %s com %d setores de 512 bytes|\n",nome_disco,SETORES);
    printf(" -----------------------------------------------\n"GRAY);
    /*Fim print inicial*/
    while(1) { //loop infinito
        printf("# ");
        fflush(stdout);

        if(!fgets(line,sizeof(line),stdin))
            break;

        if(line[0]=='\n') 
            continue;
        line[strlen(line)-1] = 0;

        args = sscanf(line,"%s %s",cmd,arg);//recebe comando e argumento
        fflush(stdout);
        if(args==0) 
            continue;
        //inicio if e elses dos comandos
        if(strcmp(cmd,"sair")==0) {
            break;
        }
        else if(strcmp(cmd,"ajuda")==0) {
            printf(BOLD"Comandos:\n");
            printf("criad caminho\\nome_do_diretorio ->cria diretorio\n");
            printf("criaa caminho\\nome_do_arquivo tamanho ->cria arquivo\n");
            printf("removed caminho\\nome_do_diretorio ->deleta diretorio\n");
            printf("removea caminho\\nome_do_arquivo ->deleta arquivo\n");
            printf("verd caminho ->mostra diretorio\n");
            printf("verset caminho\\nome_do_arquivo ->mostra granulos ocupados pelo arquivo\n");
            printf("mapa ->mostra tabela de granulos\n");
            printf("arvore ->arvore de diretorios\n");
            printf("ajuda ->listar comandos\n");
            printf("sair ->sair do simulador\n"NONE);
        }
        else if(strcmp(cmd, "mapa")==0){
            mapa();
            }
        else if(strcmp(cmd,"criad")==0) {
            if(args==2) {
                criad(arg);
            }
        }
        else if(strcmp(cmd, "arvore")==0){
            struct setor setorr;
            read_disco(9, &setorr);
            arvore(0,setorr,1);
            printf("\n");
        }
        else if(strcmp(cmd,"verd")==0){
            if(args==2) {
                verd(arg);
            }
        }
        else if(strcmp(cmd,"removea")==0) {
            if(args==2) {
                removea(arg);
            }
        }
        //fim if e elses dos comandos
    }

    printf("O %s agradece! Volte sempre!.\n",nome_disco);
    fclose(diskfile);//fechar disco
    diskfile = 0;

    return 0;
}
