// -------------------------------------------------------------------------------

// Autores:

// Caio Alessandro Resnauer
// Erik Ryuichi Yamamoto
// Victor Jordão

// -------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/systick.h"
#include "utils/uartstdio.h"

#include "system_TM4C1294.h"

#include "gpio.h"
#include "UARTInit.h"

#define BIT0 (0x1)

#define BIT1 (0x2)
#define BITS_1_0 (0x3) // 2 + 1 = 3 = 0x3

#define BIT2 (0x4)
#define BITS_2_0 (0x5) // 4 + 1 = 5 = 0x5
#define BITS_2_1 (0x6) // 4 + 2 = 6 = 0x6
#define BITS_2_1_0 (0x7) // 4 + 2 + 1 = 7 = 0x7

#define BIT3 (0x8)
#define BITS_3_0 (0x9) // 8 + 1 = 9 = 0x9
#define BITS_3_1 (0xA) // 8 + 2 = 10 = 0xA
#define BITS_3_1_0 (0xB) // 8 + 2 + 1 = 11 = 0xB
#define BITS_3_2 (0xC) // 8 + 4 = 12 = 0xC
#define BITS_3_2_0 (0xD) // 8 + 4 + 1 = 13 = 0xD
#define BITS_3_2_1 (0xE) // 8 + 4 + 2 = 14 = 0xE
#define BITS_3_2_1_0 (0xF) // 8 + 4 + 2 + 1 = 15 = 0xF

#define BIT4 (0x10)

#define BIT5 (0x20)
#define BITS_5_4 (0x30) // 2 0 + 1 0 = 3 0 = 0x30

#define BIT6 (0x40)
#define BITS_6_4 (0x50) // 4 0 + 1 0 = 5 0 = 0x50
#define BITS_6_5 (0x60) // 4 0 + 2 0 = 6 0 = 0x60
#define BITS_6_5_4 (0x70) // 4 0 + 2 0 + 1 0 = 7 0 = 0x70

#define BIT7 (0x80)
#define BITS_7_4 (0x90) // 8 0 + 1 0 = 9 0 = 0x90
#define BITS_7_5 (0xA0) // 8 0 + 2 0 = 10 0 = 0xA0
#define BITS_7_5_4 (0xB0) // 8 0 + 2 0 + 1 0 = 11 0 = 0xB0
#define BITS_7_6 (0xC0) // 8 0 + 4 0 = 12 0 = 0xC0
#define BITS_7_6_4 (0xD0) // 8 0 + 4 0 + 1 0 = 13 0 = 0xD0
#define BITS_7_6_5 (0xE0) // 8 0 + 4 0 + 2 0 = 14 0 = 0xE0
#define BITS_7_6_5_4 (0xF0) // 8 0 + 4 0 + 2 0 + 1 0 = 15 0 = 0xF0

// Definição dos modos do LCD
#define ModoInstrucao (BIT2) // [EN = 1, RW = 0, RS = 0] -> Modo Instrução
#define ModoEscritaDados (BITS_2_0) // [EN = 1, RW = 0, RS = 1] -> Modo Dados
#define ModoLeituraBusyEndereco (BITS_2_1) // [EN = 1, RW = 1, RS = 0] -> Modo de Leitura de Busy Flag e de valor de Endereço
#define ModoLeituraRAM (BITS_2_1_0) // [EN = 1, RW = 1, RS = 1] -> Modo de Leitura da RAM

// Definição das instruções do LCD no Modo Instrução [0 = 0 | 1 = 1 | X = Irrelevante (Não importa) | V = Variável]

// [BIT7 = 0 | BIT6 = 0 | BIT5 = 0 | BIT4 = 0 | BIT3 = 0 | BIT2 = 0 | BIT1 = 0 | BIT0 = 1]
#define LimpaLCD (BIT0) // Limpa o Display LCD

// [BIT7 = 0 | BIT6 = 0 | BIT5 = 0 | BIT4 = 0 | BIT3 = 0 | BIT2 = 0 | BIT1 = 1 | BIT0 = X]
#define ReiniciaCursor (BIT1) // Reinicia a posição do cursor

// [BIT7 = 0 | BIT6 = 0 | BIT5 = 0 | BIT4 = 0 | BIT3 = 0 | BIT2 = 1 | BIT1 = V | BIT0 = V]
#define ConfiguraCursorEsquerda (BIT2) // [Cursor move para a esquerda | Desativa deslocamento do display]
#define ConfiguraCursorDireita (BITS_2_1) // [Cursor move para a direita | Desativa deslocamento do display]
#define ConfiguraDisplayDireita (BITS_2_0) // [Display move para a direita | Ativa deslocamento do display]
#define ConfiguraDisplayEsquerda (BITS_2_1_0) // [Display move para a esquerda | Ativa deslocamento do display]

// [BIT7 = 0 | BIT6 = 0 | BIT5 = 0 | BIT4 = 0 | BIT3 = 1 | BIT2 = V | BIT1 = V | BIT0 = V]
#define DisplayD_CursorD_PiscanteD (BIT3) // [Desativa display | Desativa cursor | Desativa piscante]
#define DisplayD_CursorD_PiscanteA (BITS_3_0) // [Desativa display | Desativa cursor | Ativa piscante]
#define DisplayD_CursorA_PiscanteD (BITS_3_1) // [Desativa display | Ativa cursor | Desativa piscante]
#define DisplayD_CursorA_PiscanteA (BITS_3_1_0) // [Desativa display | Ativa cursor | Ativa piscante]
#define DisplayA_CursorD_PiscanteD (BITS_3_2) // [Ativa display | Desativa cursor | Desativa piscante]
#define DisplayA_CursorD_PiscanteA (BITS_3_2_0) // [Ativa display | Desativa cursor | Ativa piscante]
#define DisplayA_CursorA_PiscanteD (BITS_3_2_1) // [Ativa display | Ativa cursor | Desativa piscante]
#define DisplayA_CursorA_PiscanteA (BITS_3_2_1_0) // [Ativa display | Ativa cursor | Ativa piscante]

// [BIT7 = 0 | BIT6 = 0 | BIT5 = 0 | BIT4 = 1 | BIT3 = V | BIT2 = V | BIT1 = X | BIT0 = X]
#define DeslocaCursorEsquerda (BIT4) // [Desloca cursor | Deslocamento para a esquerda]
#define DeslocaCursorDireita (BIT4 | BIT2) // [Desloca cursor | Deslocamento para a direita]
#define DeslocaDisplayEsquerda (BIT4 | BIT3) // [Desloca display | Deslocamento para a esquerda]
#define DeslocaDisplayDireita (BIT4 | BITS_3_2) // [Desloca display | Deslocamento para a direita]

// [BIT7 = 0 | BIT6 = 0 | BIT5 = 1 | BIT4 = V | BIT3 = V | BIT2 = V | BIT1 = X | BIT0 = X]
#define Modo_4Bits_1Linha_5x8Pontos (BIT5) // [Barramento 4 bits | Display em 1 linha | Caractere com 5x8 pontos]
#define Modo_4Bits_1Linha_5x11Pontos (BIT5 | BIT2) // [Barramento 4 bits | Display em 1 linha | Caractere com 5x11 pontos]
#define Modo_4Bits_2Linhas_5x8Pontos (BIT5 | BIT3) // [Barramento 4 bits | Display em 2 linhas | Caractere com 5x8 pontos]
#define Modo_4Bits_2Linhas_5x11Pontos (BIT5 | BITS_3_2) // [Barramento 4 bits | Display em 2 linhas | Caractere com 5x11 pontos]
#define Modo_8Bits_1Linha_5x8Pontos (BITS_5_4) // [Barramento 8 bits | Display em 1 linha | Caractere com 5x8 pontos]
#define Modo_8Bits_1Linha_5x11Pontos (BITS_5_4 | BIT2) // [Barramento 8 bits | Display em 1 linha | Caractere com 5x11 pontos]
#define Modo_8Bits_2Linhas_5x8Pontos (BITS_5_4 | BIT3) // [Barramento 8 bits | Display em 2 linhas | Caractere com 5x8 pontos]
#define Modo_8Bits_2Linhas_5x11Pontos (BITS_5_4 | BITS_3_2) // [Barramento 8 bits | Display em 2 linhas | Caractere com 5x11 pontos]

// [BIT7 = 0 | BIT6 = 1 | (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0) = V]
#define CarregaEnderecoCaractere (BIT6) // Carrega o contador de endereço com o endereço do gerador de caractere fornecido (6 bits - BIT0 até BIT5)

// [BIT7 = 1 | (BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0) = V]
#define MoveCursorLinha1 (BIT7) // Carrega o contador de endereço com o endereço de display fornecido (7 bits - BIT0 até BIT6) [Linha 1]
#define MoveCursorLinha2 (BITS_7_6) // Carrega o contador de endereço com o endereço de display fornecido (7 bits - BIT0 até BIT6) [Linha 2]


// Definição do tempo para alternar mensagens
#define TempoMensagem (1000)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TAM_VET (2000) //constante que determina o tamanho do vetor de leituras

uint16_t i_vet; //Indice utilizado em loop para percorrer o vetor de leituras
bool vet[TAM_VET]; //vetor que armazena as ultimas leituras
uint8_t num_transicoes; //variável utilizada para contar o número de transições/bordas
uint8_t num_baixos_altos[2]; //vetor para contar a quantidade de leituras em baixas e altas
float k; //constante para conversão para milisegundos
uint8_t n_altos; //armazena a quantidade de leituras altas
uint8_t n_baixos; //armazena a quantidade de leituras baixas
float periodo; //armazena o calculo do período
float frequencia; //armazena o calculo da frequência
float duty_cycle; //armazena o calculo do duty cycle

void computaResultados() 
{  
    k = 3*1000/24000000;
    n_baixos = num_baixos_altos[0];
    n_altos = num_baixos_altos[1];
    periodo = k * (n_baixos + n_altos);
    frequencia = 1 / periodo;
    duty_cycle = n_altos / (n_altos + n_baixos);
    
    UARTprintf("Duty cycle: %f\%\%\n",duty_cycle);
    UARTprintf("Periodo: %fs\n",periodo);
    UARTprintf("Frequencia: %fhz\n\n",frequencia);
}

void main(void){
  
  GPIO_Init();
  UARTInit();
  
  while(1){
    i_vet = 0;
    
    while(i_vet < TAM_VET) {
      vet[i_vet] = PortJ_Input() & BIT0;
      i_vet++;
    }
    
    i_vet = 1;
    num_transicoes = 0;
    num_baixos_altos[0] = 0;
    num_baixos_altos[1] = 0;
    
    while(i_vet < TAM_VET) {
      if(num_transicoes > 0) {
        if(num_transicoes == 3) {
          break;
        }
        
        else {
          num_baixos_altos[vet[i_vet - 1]]++;
        }
      }
      
      if(vet[i_vet] != vet[i_vet - 1]) {
        num_transicoes++;
        
      }
      
      i_vet++;
    }
    
    if(num_transicoes > 0) {
          computaResultados();
    }
    else {
      UARTprintf("Erro: Nenhuma borda detectada\n");
      
    }
  }
} // main