# Atividades_Sistemas_Operacionais_II
Atividades e projetos feitos na linguagem C, para avaliação na disciplina Sistemas Operacionais II (SO2)

## 🚀 Atividade feitas individualmente
* MyMiniShell
* Thread Barbeiro
* Projeto 1
  
## 🚀 Atividades em grupo
* Projeto 2
  * Lucas Bastelli
  * Pedro Gonçalves
  
## 📋 Descrição de cada atividade
### MyMiniShell
1. Exibe um prompt e aguarda entrada do usuário 
2. O comando exit deve sair do processador de comandos 
3. Quando a entrada é o nome de um programa no path ou o caminho de  um programa, o mesmo deve ser executado 
4. Quando a entrada não é um programa no path ou caminho válido, deve  ser mostrada uma mensagem de erro adequada 
5. Os argumentos digitados na linha de comando devem ser passados ao  programa chamado

### Thread Barbeiro
– Uma barbearia tem n barbeiros com suas respectivas cadeiras de barbeiro e m cadeiras para  clientes esperarem por sua vez na sala de espera 

– Quando não há clientes, o barbeiro se senta na cadeira e dorme 

– Quando chega um cliente: 
    
    • Se um barbeiro está disponível, ele precisa acordá-lo 
    
    • Se todos os barbeiros estão ocupados, o cliente senta-se em uma das cadeiras na sala de espera e espera sua  vez 
    
    • Se não há cadeiras disponíveis na sala de espera, o cliente vai embora

### Projeto 1
Foi implementado um processador de comandos do Linux em C, que possui as seguintes características:

- Exibe um prompt e aguarda entrada do usuário
- O comando exit deve sair do processador de comandos.
- Quando a entrada do usuário é o nome de um programa no path ou o caminho
completo ou relativo de um programa, o mesmo deve ser executado.
- Quando a entrada do usuário não é um programa ou comando válido, deve ser 
mostrada uma mensagem de erro adequada.
- Os argumentos digitados na linha de comando devem ser passados ao programa 
que será executado.
- O prompt deve ter o formato: [MySh] nome-de-usuario@hospedeiro:diretorio-atual$.
- No prompt, o caminho do diretório home do usuário deve ser substituído por ~, 
caso este faça parte do caminho atual. Exemplo: de /home/juquinha/aula para 
~/aula.
- Implemente o comando cd para mudar diretórios (quando recebe argumento) ou 
voltar ao diretório home do usuário (sem argumentos ou com o argumento ~).
- Mostre uma mensagem de erro adequada se cd falhar.
- Ctrl+Z e Ctrl+C não devem causar a parada ou interrupção do processador de 
comandos (apenas ignore a linha com o comando).
- Ctrl+D deve sair do processador de comandos, tal qual se o comando exit
tivesse sido utilizado.
- O processador de comandos deve permitir o uso de pipes. O símbolo | indica a 
separação entre cada programa, conectando a saída padrão do programa à esquerda 
com a entrada padrão do programa à direita através de um pipe.

### Projeto 2
Foi implementado um simulador de um sistema de arquivos, utilizando a linguagem C, que possui as seguintes características:

- Simular um disco com 256 setores de 512 bytes. Considere que os 10 primeiros setores são ocupados 
pelo setor de boot, sistema de arquivos e o diretório raiz, ficando os demais setores disponíveis para dados 
do usuário.
- O simulador deve permitir a criação de diretórios e subdiretórios com um comando criad até o 
mínimo de 8 níveis. Cada diretório pode conter vários subdiretórios. Cada diretório/subdiretório deve 
ocupar um setor do disco. Nomes de subdiretórios precisam ser únicos dentro de um diretório.
- O simulador deve permitir a exclusão de diretórios utilizando um comando removed. O simulador não 
deve permitir a exclusão de diretórios que não estejam vazios.
- O simulador deve oferecer um comando verd que exibe os arquivos e subdiretórios do diretório 
indicado como parâmetro. Na listagem, os diretórios devem conter uma indicação de que são diretórios e os 
arquivos devem conter tamanho, além de data e hora de criação. No final da listagem deve aparecer o total 
de arquivos e diretórios, o total do tamanho dos arquivos e a quantidade de espaço livre no disco. Lembre-se 
que o espaço vazio dos setores utilizados não pode ser utilizado por outros arquivos ou diretórios, portanto 
não devem entrar na conta do espaço livre. Também deve ser indicado o tamanho total alocado para 
arquivos e diretórios, incluindo aí o espaço perdido com fragmentação (espaço não usado em setores 
ocupados). Obs.: Função de exibir data e hora não foi feita.
- O simulador deve oferecer um comando mapa que mostre graficamente quais são os setores ocupados 
e quais são os setores livres.
- O simulador deve oferecer um comando arvore que mostre graficamente a árvore de diretórios do 
sistema de arquivos.
- O simulador deve oferecer um comando ajuda que quando utilizado mostra quais são os comandos 
disponíveis no sistema.
- O simulador deve fornecer um comando sair que pode ser usado para encerrar o simulador.

## ⚙️ Arquivos
* myminish.c - Código referente a atividade MyMiniShell
* barbeiro.c - Código referente a atividade Thread Barbeiro
* mysh.c - Código referente ao Projeto 1
* p2.c - Código referente ao Projeto 2

