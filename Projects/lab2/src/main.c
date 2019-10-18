// -------------------------------------------------------------------------------

// Autores:

// Erik Ryuichi Yamamoto
// Victor Jordão

// -------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/systick.h"
#include "utils/uartstdio.h"

#include "system_TM4C1294.h"

#include "gpio.h"
#include "UARTInit.h"
#include "temporizador.h"

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TAM_VET (24000) //constante que determina o tamanho do vetor de leituras

uint16_t i_vet; //Indice utilizado em loop para percorrer o vetor de leituras
bool vet[TAM_VET]; //vetor que armazena as ultimas leituras
uint8_t num_transicoes; //variável utilizada para contar o número de transições/bordas
uint16_t num_baixos_altos[2]; //vetor para contar a quantidade de leituras em baixas e altas
double k; //constante para conversão para milisegundos
uint16_t n_altos; //armazena a quantidade de leituras altas
uint16_t n_baixos; //armazena a quantidade de leituras baixas
double periodo; //armazena o calculo do período
double frequencia; //armazena o calculo da frequência
double duty_cycle; //armazena o calculo do duty cycle

uint32_t t_altos[TAM_VETOR];      //armazena a quantidade de leituras altas
uint32_t t_baixos[TAM_VETOR];     //armazena a quantidade de leituras baixas;
double duty_cycle_1;            //Armazena o calculo do duty cycle

uint8_t freq_decimal;
uint8_t duty_cycle_decimal;

void computaResultados() {
  k = (-0.000000627) * (SystemCoreClock - 24000000) + 91.3; //31.1; //91.3;//3*1000/24000000;
  n_baixos = num_baixos_altos[0];
  n_altos = num_baixos_altos[1];
  periodo = k * (n_baixos + n_altos);
  frequencia = (double)(1000000 / periodo);
  freq_decimal = (frequencia - (uint32_t)frequencia) * 100;
  duty_cycle = ((double)n_altos / (n_altos + n_baixos)) * 10000;
  duty_cycle_decimal = (duty_cycle - (uint32_t)duty_cycle) * 100;
  duty_cycle = duty_cycle / 100;
  
  UARTprintf("Altos: %d\n", n_altos);
  UARTprintf("Baixos: %d\n", n_baixos);
  UARTprintf("Periodo: %d ns\n",(uint32_t)periodo);
  UARTprintf("Frequencia: %d,%d kHz\n", (uint32_t)frequencia, freq_decimal);
  UARTprintf("Duty cycle: \%d,%d\%\%\n\n", (uint8_t)duty_cycle, (uint8_t)duty_cycle_decimal);
}

void adquireAmostras() {
  while(i_vet < TAM_VET) {
    vet[i_vet] = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0);
    PortA_Output(PortC_Input());
    i_vet++;
  }
}

void contaBaixosAltos() {
  while(i_vet < TAM_VET) {
    if(num_transicoes > 0) {
      if(num_transicoes == 101) {
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
}

bool entrada_D0 = 0;

void main(void){
  GPIO_Init();
  UARTInit();
  TimerInit();
  PortM_Output(BIT6);
  
  while(1){
    if(amostragemPronta()) {
      //calcula();
      UARTprintf("Duty cycle: \%d\%\%\n", duty_cycle_1);
      UARTprintf("Tempo Alto: \%d\n", t_altos[0]);
      UARTprintf("Tempo Baixo: \%d\n\n", t_baixos[0]);
      resetaAmostragemPronta();
    }
    /*
    if(num_transicoes > 0) {
       computaResultados();
    }
    else {
      UARTprintf("Erro: Nenhuma borda detectada\n");
      
    }*/
    //valorTimer = TimerValueGet64(TIMER0_BASE);
    //PortA_Output(valorTimer >> 60);
    //entrada_D0 = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0);
  }
} // main