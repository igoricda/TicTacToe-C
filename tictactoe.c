//Igor Correa Domingues de Almeida

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Codigo para limpar a tela
#define CLEAR "\033[H\033[2J"

// Variaveis Globais
char board[3][3] = {'_', '_', '_', '_', '_', '_', '_', '_', '_'}; // Tabuleiro do jogo
char clone0[3][3], clonex[3][3]; //Clones para uso do modo dificil
short wins[2] = {0, 0};                  // Contador de pontos de cada jogador
short currentplayer = 1;                 // Jogador atual
short l = 0, c = 0;  // Linha e coluna onde a jogada sera feita
short rodada = 0;    // Numeros de rodada realizadas (max 9)
short game = 0;      // Contar a quantidade de jogos
short endgame = 0;   // Sinalizador que alguem ganhou
char resposta = 's'; //Condicao para parar de jogar
char sg; // Escolha se quer comecar o jogo no pvc
short gamemode, gamediff, wc = 0; //Variaveis para modo de jogo e dificuldade e se alguem ganhou
short l1 = -1, c1 = -1, l2 = -1, c2 = -1; //Variaveis para salvar as posicoes dos clones

// Funcao para limpar o board para o proximo jogo
void LimparBoard() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = '_';
    }
  }
}

//Clone do board para os testes do minimax
void assignarray() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      clone0[i][j] = board[i][j];
      clonex[i][j] = board[i][j];
    }
  }
}

// Funcao para limpar a tela UNIX
void clrscr() {
  printf(CLEAR);
  return;
}

// Funcao wait(): Espera até teclar [ENTER]
void wait() {
  printf("\nTecle [ENTER]\n");
  getchar();
  getchar();
}

// Funcao para escolha do modo de jogo
void pvporpvc() {
  printf("Escolha o modo de jogo:\n1 - Player versus Player\n2 - Player vs "
         "Computador\n");
  //Se escolher uma opcao invalida perguntar novamente
  while (scanf("%hd", &gamemode) && (gamemode != 1 && gamemode != 2)) {
    printf("\nResposta inválida\n");
    wait(); //Limpar o buffer
    clrscr();
    printf("Escolha o modo de jogo:\n1 - Player vs Player\n2 - Player vs Computador\n");
  }
}

// Funcao para escolher a dificuldade do jogo
void dificuldade() {
  clrscr();
  printf("Você escolheu o modo PvC!\nEscolha a dificuldade:\n1 - Fácil\n2 - Difícil\n");
  //Se escolher uma opcao invalida perguntar novamente
  while (scanf("%hd", &gamediff) && (gamediff != 1 && gamediff != 2)) {
    printf("\nResposta inválida\n");
    wait(); //Limpar buffer
    clrscr();
    printf("Você escolheu o modo PvC!\nEscolha a dificuldade:\n1 - Fácil\n2 - Difícil\n");
  }
}

//Funcao para explicar ao jogador o funcionamento da pontuacao
void pontuacao(){
  clrscr(); //Limpar a tela
  printf("Pontuação:\n\nEm vitórias, o vencedor ganha 2 pontos\nEm empate, ambos ganham 1 ponto\nEm derrota, ganha 0 pontos\n"); //Explicacao
  printf("\nTecle [ENTER]\n"); //Mesmo da funcao wait() mas com um getchar() a menos
  getchar();
}

// Funcao de impressao da interface principal para PvP (N° do jogo, vitorias
// e identificacao dos players e n° da rodada)
void printinterfpvp() {
  printf("JOGO: %d\nPLAYER 1: 0 (%d PONTOS)\nPLAYER 2: X (%d PONTOS)\nRODADA: "
         "%d\n\n", game, wins[0], wins[1], rodada);
  if(currentplayer == 1)
    printf("PLAYER 1 JOGA:\n");
  else
    printf("PLAYER 2 JOGA:\n");
}

// Funcao de impressao da interface principal para PvC (N° do jogo, vitorias
// e identificacao dos players e n° da rodada)
void printinterfpvc() {
  if (gamediff == 1) //Imprimir a dificuldade escolhida
    printf("DIFICULDADE: FÁCIL\n");
  else
    printf("DIFICULDADE: DIFÍCIL\n");
  printf("JOGO: %d\nPLAYER: 0 (%d PONTOS)\nCOMPUTADOR: X (%d "
         "PONTOS)\nRODADA: %d\n\n",
         game, wins[0], wins[1], rodada);
}
// Funcao de imprimir o tabuleiro
void printboard() {

  printf("+---+---+---+\n");
  printf("| %c | %c | %c |\n", board[0][0], board[0][1], board[0][2]);
  printf("+---+---+---+\n");
  printf("| %c | %c | %c |\n", board[1][0], board[1][1], board[1][2]);
  printf("+---+---+---+\n");
  printf("| %c | %c | %c |\n", board[2][0], board[2][1], board[2][2]);
  printf("+---+---+---+\n");
}

// Funcao para agrupar os elementos visuais do jogo
void printgame() {
  if (gamemode == 1) //Dependendo do modo, printar uma interface diferente
    printinterfpvp();
  else
    printinterfpvc();
  printboard();
}

// Funcao para pedir a linha e coluna da jogada para o usuario
void play() {
  printf("\n\nLINHA DA JOGADA: ");
  scanf("%hd", &l);
  printf("\nCOLUNA DA JOGADA: ");
  scanf("%hd", &c);
}

//Verificar se alguem ganhou e quem
void wincon(char board[3][3]){
  if ((board[0][0] == 'X' && board[1][1] == 'X' && board[2][2] == 'X') ||
    (board[0][2] == 'X' && board[1][1] == 'X' && board[2][0] == 'X') ||
    (board[0][0] == 'X' && board[0][1] == 'X' && board[0][2] == 'X') ||
    (board[1][0] == 'X' && board[1][1] == 'X' && board[1][2] == 'X') ||
    (board[2][0] == 'X' && board[2][1] == 'X' && board[2][2] == 'X') ||
    (board[0][0] == 'X' && board[1][0] == 'X' && board[2][0] == 'X') ||
    (board[0][1] == 'X' && board[1][1] == 'X' && board[2][1] == 'X') ||
    (board[0][2] == 'X' && board[1][2] == 'X' && board[2][2] == 'X')) {
      wc = 1;
  }
  else if ((board[0][0] == '0' && board[1][1] == '0' && board[2][2] == '0') ||
    (board[0][2] == '0' && board[1][1] == '0' && board[2][0] == '0') ||
    (board[0][0] == '0' && board[0][1] == '0' && board[0][2] == '0') ||
    (board[1][0] == '0' && board[1][1] == '0' && board[1][2] == '0') ||
    (board[2][0] == '0' && board[2][1] == '0' && board[2][2] == '0') ||
    (board[0][0] == '0' && board[1][0] == '0' && board[2][0] == '0') ||
    (board[0][1] == '0' && board[1][1] == '0' && board[2][1] == '0') ||
    (board[0][2] == '0' && board[1][2] == '0' && board[2][2] == '0')) {
      wc = 2;
  }
}

// Imprimir o vencedor ou se deu empate e somar os pontos
void printwin() {
  //Condicao de vitoria player 2 / Computer
  if (wc == 1) {
    wins[1] += 2; //Contador de pontos
    rodada -= 1; // Para aparecer na interface a ultima rodada jogada
    clrscr(); //Limpar a tela
    printgame(); //Imprimir interface e tabuleiro

    if (gamemode == 1)// Diferenciar entre Player 2 e computador
      printf("\nPLAYER 2 VENCE\n");  //Resultado da partida
    else
      printf("\nCOMPUTADOR VENCE\n"); //Resultado da partida

    endgame = 1; // Condicao de parada do laco

    if(rodada == 10) // Para nao mostrar "RODADA 10" na interface
      rodada = 9;

  } //Condicao de vitoria player 1
  else if (wc == 2) {
    wins[0] += 2; //Contador de pontos
    clrscr(); // Limpar a tela
    rodada -= 1; // Para aparecer na interface a ultima rodada jogada
  if(rodada == 10) // Para nao mostrar "RODADA 10" na interface
    rodada = 9;
    printgame(); // Print tabuleiro e interface
    if (gamemode == 1)
      printf("\nPLAYER 1 VENCE\n"); //Resultado da partida
    else
      printf("\nPLAYER VENCE\n");
    endgame = 1; // Condicao de parada do laco
  } 
  else if (rodada >= 10) { //Caso rodada maior que 9 e ninguem tiver ganho
    clrscr(); //Limpar a tela
    rodada = 9; // Para nao mostrar "RODADA 10" na interface
    wins[0]++; //Contador de pontos
    wins[1]++; //Contador de pontos 
    printgame(); //Printar tabuleiro e interface
    printf("\nEMPATE\n"); //Resultado da partida 
    endgame = 1; //Condicao de parada do laco
  }
}

//Funcao juntando condicao de vitoria e a impressao em caso de vitoria
void wincondition(){
  wincon(board);
  printwin();
}

// Funcao para computar a jogada e verificar se ela é valida
void jogar() {
  if ((board[l - 1][c - 1] != '_') || ((c < 1) || (c > 3)) ||
      ((l < 1) || (l > 3))) {
    printf("\nEscolha inválida\n");
  } else {
      if (currentplayer == 2) {
          board[l - 1][c - 1] = 'X'; //Se valido, preencher
          currentplayer = 1;         //Trocar de jogador
      } else {
          board[l - 1][c - 1] = '0'; //Se valido, preencher
          currentplayer = 2;         //Trocar de jogador
      }
      rodada++; //Incremento da rodada
  }
  wait();
}

//Funcao de jogo do PvC, só as jogadas do player, e sua validade
void jogarvsc() {
    if ((board[l - 1][c - 1] != '_') || ((c < 1) || (c > 3)) ||
        ((l < 1) || (l > 3))) {
      printf("\nEscolha inválida\n");
    } 
    else {
        board[l - 1][c - 1] = '0'; //Se valido, preencher
      rodada++; //Incremento da rodada
    }
    wait();
}

//Funcao para escolher um numero aleatorio
void randomize(){
  srand(time(NULL)); //Funcao para escolher numeros aleatorios
  c = rand() % 3; //Numero aleatorio de 0 a 2 para coluna
  l = rand() % 3; // Numero aleatorio de 0 a 2 para linha
}

//Funcao para sortear a jogada do computador se comecar jogando
void compfacil1(){
  randomize();
  //Se a posicao estiver ocupada e o jogo nao ter acabado, sortear novamente 
  while(board[l][c] != '_' && rodada < 9){ 
    randomize();
  }
  if(rodada % 2 == 0 && rodada < 9){ //A posicao sorteada recebe 'X', se a rodada for par e menor que 9
    board[l][c] = 'X';
    rodada++; //Incremento da rodada
  }
}

//Funcao modo facil sem comecar jogando
void compfacil2(){
  randomize();
  //Se a posicao estiver ocupada e o jogo nao ter acabado, sortear novamente 
  while(board[l][c] != '_' && rodada < 10){ 
    randomize();
  }
  if((rodada % 2 == 1) && rodada < 10){ //A posicao sorteada recebe 'X', se a rodada for par e menor que 10
    board[l][c] = 'X';
    rodada++; //Incremento da rodada
  }
}

//Funcao modo dificil do jogo, player começa jogando
void compdificil1(){
  if(rodada == 2){ //Primeira jogada do computador
    if(board[1][1] == '_'){ //Teste se o quadrado do meio esta livre
      board[1][1] = 'X';
    }
    else{ //Preencher o primeiro quadrado
      board[0][0] = 'X';
    }
      rodada++; //Incremento da rodada
  }
  else if((rodada % 2 == 0) && rodada < 9){ //Rodadas do bot
    //Teste de todas as posicoes para ver a possibilidade de ganhar ou perder
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
        wc = 0; //Condicao para a jogada defensiva funcionar
        assignarray(); //Funcao para voltar o board ao padrao apos cada teste
        if(board[i][j] == '_'){ //Se o espaço esta livre
          clonex[i][j] = 'X'; //Teste no clone de vitoria
          wincon(clonex); //Teste se ganhara
          if(wc == 1){ //Armazenar a posicao que ganha
            l1 = i; 
            c1 = j;
          }
          clone0[i][j] = '0'; //Teste no clone de derrota
          wincon(clone0); //Teste se perdera
          if(wc == 2){ //Armazenar a posicao que perde
            l2 = i;
            c2 = j;
          }
        }
      }
    }
    //Prioridade de posicao ganhadora
    if((l1 >= 0 && l1 <= 3) && (board[l1][c1] == '_')){
      board[l1][c1] = 'X';
    }
    //Senao, prioridade de preencher a posicao perdedora
    else if((l2 >= 0 && l2<= 3) && (board[l2][c2] == '_')){
      board[l2][c2] = 'X';
    }
    //Se for na rodada 4
    else if(rodada == 4){
      //Para parar combinacoes especificas, quando o bot tem o meio
      if(board[1][1] == 'X'){
        if((board[0][0] == '0' && board[2][2] == '0')
            || (board[2][0] == '0' && board[0][2] == '0')){
          board[0][1] = 'X';
        }
        else if((board[0][0] == '0' || board[2][2] == '0')){
          if(board[2][1] == '0' || board[1][0] == '0')
            board[2][0] = 'X'; 
          else if(board[1][2] == '0' || board[0][1])
            board[0][2] = 'X';
        }
        else if(board[0][2] == '0' || board[2][0] == '0'){
          if(board[1][2] == '0' || board[2][1] == '0')
            board[2][2] = 'X';
          else if(board[0][1] == '0' || board[1][0])
            board[0][0] = 'X';
        }//Quando escolhe 2 posicoes de meio
        else if(board[0][1] == '0' && board[1][2] == '0')
          board[0][2] = 'X';
        else if(board[0][1] == '0' && board[1][0] == '0')
          board[0][0] = 'X';
        else if(board[1][0] == '0' && board[2][1] == '0')
          board[2][0] = 'X';
        else if(board[2][1] == '0' && board[1][2] == '0')
          board[2][2] = 'X';
        else if(board[1][0] == '0' && board[1][2] == '0')
          board[0][1] = 'X';
        else if(board[0][1] == '0' && board[2][1] == '0')
          board[1][0] = 'X';
      }
      //Quando o bot nao tem o meio
      else if(board[2][2] == '0' && board[1][1] == '0')
          board[2][0] = 'X';
      else{ //Para caso nada seja atendido, o computador fazer alguma jogada
        compfacil1();
        rodada--;
      }
    } //No caso de escolher 2 posicoes de meio, e uma terceira posicao, que nao ativa a wincondition
    else if(rodada == 6){ 
      if((board[0][1] == '0' && board[1][2] == '0' && board[2][0] == '0') ||
         (board[1][0] == '0' && board[1][2] == '0' && board[2][1] == '0') ||
         (board[0][1] == '0' && board[2][1] == '0' && board[1][2] == '0'))
        board[0][0] = 'X';
      else if(board[0][1] == '0' && board[1][0] == '0' && board[2][2] == '0')
        board[2][0] = 'X';
      else if(board[1][0] == '0' && board[2][1] == '0' && board[0][2] == '0')
        board[2][2] = 'X';
      else if(board[2][1] == '0' && board[1][2] == '0' && board[0][0] == '0')
        board[0][2] = 'X';
      else if((board[0][0] == '0' && board[0][2] == '0' && board[2][1] == '0') || (board[2][0] == '0' && board[2][2] == '0' && board[0][1] == '0'))
        board[1][0] = 'X';
      else if((board[0][0] == '0' && board[2][0] == '0' && board[1][2] == '0') || (board[0][2] == '0' && board[2][2] == '0' && board[1][0] == '0'))
        board[0][1] = 'X';
      else{ //Quando nao ativa wincondition, mas tambem nao atende as anteriores
        compfacil1();
        rodada--;
      }
    }
    //Se nenhum dos acima, preencher uma aleatoria
    else{
      compfacil1();
      rodada--; //Decremento devido a funcao
    }
    rodada++;
    wc = 0; //Reset do wc
  }
}

//Funcao para o modo dificil, com o bot jogando primeiro
void compdificil2(){ 
  if(rodada == 1){ //Primeira jogada do computador
      board[2][0] = 'X';
      rodada++; //Incremento da rodada
  }
  if(rodada == 3){ //Na rodada 3
    if(board[1][1] == '0') //Se o meio estiver preenchido, ir na diagonal oposta
      board[0][2] = 'X';
    else if(board[1][0] == '0' || board[0][0] == '0' || board[0][1] == '0' || board[0][2] == '0') //Preencher o canto oposto a jogada do player
      board[2][2] = 'X';
    else
      board[0][0] = 'X';
    rodada++;
  }
  else if((rodada % 2 == 1) && rodada < 10){ //Rodadas do bot
    //Teste de todas as posicoes para ver a possibilidade de ganhar ou perder
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
        wc = 0; //Condicao para a jogada defensiva funcionar
        assignarray(); //Funcao para voltar o board ao padrao apos cada teste
        if(board[i][j] == '_'){ //Se o espaço esta livre
          clonex[i][j] = 'X'; //Teste no clone de vitoria
          wincon(clonex); //Teste se ganhara
          if(wc == 1){ //Armazenar a posicao que ganha
            l1 = i; 
            c1 = j;
          }
          clone0[i][j] = '0'; //Teste no clone de derrota
          wincon(clone0); //Teste se perdera
          if(wc == 2){ //Armazenar a posicao que perde
            l2 = i;
            c2 = j;
          }
        }
      }
    }
    //Prioridade de posicao ganhadora
    if((l1 >= 0 && l1 <= 3) && (board[l1][c1] == '_')){
      board[l1][c1] = 'X';
    }
    //Senao, prioridade de preencher a posicao perdedora
    else if((l2 >= 0 && l2<= 3) && (board[l2][c2] == '_')){
      board[l2][c2] = 'X';
    }
    //Se for na rodada 5
    else if(rodada == 5){
      if(board[0][0] == '0' || board[2][2] == '0') // Se um desses cantos for preenchido, ir para a diagonal oposta
        board[0][2] = 'X';
      else if(board[1][1] == '_') //Se nao der, preencher o meio
        board[1][1] = 'X';
    }
    //Se nenhum dos acima, preencher uma aleatoria
    else{
      compfacil2();
      rodada--; //Decremento devido a funcao
    }
    rodada++;
    wc = 0; //Reset do wc
  }
}


// Funcao para perguntar se quer mais uma partida
void anothergame() {
  printf("\nQuer jogar outra vez (s/n)? ");
  //Se resposta diferente de 's' ou 'n', perguntar de novo
  while (scanf("%c", &resposta) && (resposta != 'n' && resposta != 's')) {
    printf("\nResposta inválida\n");
    wait(); //Limpar o buffer
    clrscr(); 
    printgame();
    printf("\nQuer jogar outra vez (s/n)? ");
  }
}

//Funcao para perguntar se quer comecar o jogo
void startgame(){
  printf("Você quer ser o primeiro a jogar (s/n)? ");
  while (scanf("%c", &sg) && (sg != 'n' && sg != 's')) {
    printf("\nResposta inválida\n");
    wait(); //Limpar o buffer
    clrscr(); 
    printf("Você quer ser o primeiro a jogar (s/n)? ");
    }
}

//Funcao para quando acabar o jogo, imprimir o resultado
void end(){
  //Mostra a quantidade de jogos, os pontos de cada e quem ganhou
  printf("JOGOS: %d\n", game);
  if(gamemode == 1){ //Diferenciar o modo de jogo para mostrar player ou computador
    printf("PLAYER 1: %d PONTOS\n", wins[0]);
    printf("PLAYER 2: ");
  }
  else{
    printf("PLAYER: %d PONTOS\n", wins[0]);
    printf("COMPUTADOR: ");
  }
  printf("%d PONTOS\n\n", wins[1]);

  //Mostrar se alguem ganhou e quem
  if(wins[0] > wins[1]){
    if(gamemode == 1) //Dependendo do modo de jogo, mostrar player 1 ou só player
      printf("PLAYER 1 VENCEU\n");
    else
      printf("PLAYER VENCEU\n");
  }
  else if(wins[1] > wins[0]){ //Dependendo do modo de jogo, mostrar player 2 ou computador
    if(gamemode == 1)
      printf("PLAYER 2 VENCEU\n");
    else
      printf("COMPUTADOR VENCEU\n");
  }
  else
    printf("EMPATE\n");
}
//Inicio do jogo
int main() {

  // Escolha de modo de jogo
  pvporpvc();

  // Escolha de dificuldade caso PvC
  if (gamemode == 2) {
    wait(); //Limpar o buffer
    clrscr(); //Limpar a tela
    dificuldade(); //Perguntar a dificuldade
  }
  wait();
  pontuacao(); //Funcao para explicar o jogador como funciona a pontuacao

  // Laco para se quiser jogar outra vez
  while (resposta == 's') {
    clrscr();
    game++;        // Aumentar o numero de jogos
    LimparBoard(); // Limpar o Board para o jogo
    rodada = 1;    // Retornar a rodada 1
    wc = 0;
    if(gamemode == 2)
      startgame(); //Escolha se quer comecar o jogo

    //Laco do jogo, rodada a rodada
    while (endgame == 0) {
    //Condicao para nao salvar o padrao do sistema no modo dificil
      l1 = -1; c1 = -1; l2 = -1; c2 = -1;
      clrscr(); //Limpar a tela
      if(sg == 'n'){ //Primeira jogada do computador
        if(gamediff == 1){
          compfacil2(); //Aletorio em rodadas impares
        }
        else{
          compdificil2(); //Modo dificil em rodadas impares
        }
      }
      printgame(); //Printar a interface
      play(); //Selecao de linha e coluna para jogar

      //Por nao inicializar a variavel currentplayer dentro da funcao int main(), a
      // cada jogo alterna o jogador que começa jogando
      if (gamemode == 1) { // Se PvP, sera por esse caminho
        jogar(); //Jogada de cada player
        wincondition(); // Verifica se alguem ganhou
      } 

      else { //Se PvC
          jogarvsc();  //Jogadas somente do player
          wincondition(); //Teste se ganhou

          if(endgame == 1)
            break; //Se ganhou, ir para a interface escolha de novo jogo

          if(gamediff == 1){ //Se estiver no modo facil
            if(sg == 's')
              compfacil1(); //Jogadas do computador no modo facil em rodadas pares (O player comeca)
            else
              compfacil2();
            wincondition(); //Verificar se ele ganhou
          }
          else{ //Se estiver no modo dificil
            if(sg == 's')
              compdificil1(); //Jogadas do computador no modo dificil em rodadas pares (O player comeca)
            else
              compdificil2();
            wincondition(); //Verificar se ele ganhou
          }
        } 
      }
    anothergame(); //Perguntar se quer jogar outra vez
    wait();
    endgame = 0; // Para poder voltar ao laco do jogo, caso a resposta seja sim
  }
  clrscr(); 
  //Mostra a quantidade de jogos, os pontos de cada e quem ganhou
  end(); //Funcao de fim de jogo
  return 0;
}
