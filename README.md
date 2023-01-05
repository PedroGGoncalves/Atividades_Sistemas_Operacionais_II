# Atividades_Sistemas_Operacionais_II

## Projeto 2
### Descrição
Foi implementado um simulador de um sistema de arquivos, utilizando a linguagem C, que possui as seguintes 
características:

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

### Arquivos
