# DCC136-Capacitated-Clustering-Problem

Trabalho de Clusterização Capacitada para a Disciplina DCC136 - Inteligência Computacional UFJF

João Víctor Costa de Oliveira
Marcelo Ian Rezende Menezes
## Como executar:

### Opção 1: Pelo exec.sh

  Apenas execute o exec.sh. Digite ./exec.sh pelo terminal.
  O programa será compilado e depois cada instância pedida será executada uma vez.
  Será criado também uma pasta solExec, onde os arquivos de saída serão deixados.
  
### Opção 2: Por linha de comando

  Digite "make" no terminal e o binário main será gerado.
  formato:
  ./main INPUT_FILE OUTPUT_FILE HANDOVER ALPHA
  INPUT_FILE = Arquivo de entrada
  OUTPUT_FILE = Arquivo de saída
  HANDOVER = 1 caso o INPUT_FILE seja Handover; 0 caso contrário
  ALPHA = float. Usado 0.3 nas execuções
  
   Exemplo: ./main instances/Handover/20_10_270001 saida.txt 1 0.3
  
  "make clean" remove o ./main
  
## Arquivo de Saída
    Seed: a seed gerada para a execução
    Qualidade: a qualidade REAL da solução
    Qualidade (Handicap): às vezes a solução é inviável antes de passar pela busca local. Essa é a qualidade dessa solução (DESCONSIDERAR)
    Viabilidade: 1 para solução viável, 0 para solução inviável
    Tempo de Execução: tempo de execução em milisegundos
    
    Cluster X => ID do cluster
      [ A - B ] C => A é o peso mínimo; B peso máximo; C peso atual
      Aresta de D para -1 => Antes tinha um propósito maior, mas agora apenas indica os vértices que compõe o cluster. D => ID vértice
      Custo => Soma do peso das arestas
      
## Saída no terminal
      Um texto indicando em qual etapa da heurística está;
      Seed
      Qualidade => DESCONSIDERAR, mesma coisa do Qualidade (Handicap) do arquivo saída
      Tempo => em milisegundos
      Caso o IG termine por limitação de tempo, haverá um aviso;
      Notificação de inviabilidade caso a solução final seja inviável;
