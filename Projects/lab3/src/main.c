// -------------------------------------------------------------------------------

// Autores:

// Erik Ryuichi Yamamoto
// Victor Jordão

// -------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
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

int64_t t_altos[TAM_VETOR];      //armazena a quantidade de leituras altas
int64_t t_baixos[TAM_VETOR];     //armazena a quantidade de leituras baixas;

uint32_t leiturasAltos[TAM_VETOR];
uint32_t leiturasBaixos[TAM_VETOR]; 

double duty_cycle;            //Armazena o calculo do duty cycle

double media_alto;            //Armazena a media dos tempos altos
double media_baixo;           //Armazena a media dos tempos baixos

double periodoTiva_ns;
double periodo;
double periodo_alto;
double periodo_baixo;
double frequenciaTiva_MHz;
double frequencia;

uint8_t freq_decimal;
uint8_t duty_cycle_decimal;

void trataLeituras(void) {
  uint16_t i;
  
  t_altos[0] = leiturasAltos[0];
  
  for(i = 1; i < TAM_VETOR; i++) {
    if(leiturasBaixos[i] > leiturasAltos[i]) {
      t_altos[i] = leiturasBaixos[i] - leiturasAltos[i];
    }
    
    if(leiturasAltos[i] > leiturasBaixos[i - 1]) {
      t_baixos[i] = leiturasAltos[i] - leiturasBaixos[i - 1];
    }
  }
}

void calcula()
{
  //Zera as variaveis
  uint16_t t = 0;
  
  media_alto = 0;
  media_baixo = 0;
  
  //Divide pela quantidade de leituras para obter a media
  for(t = 2; t < TAM_VETOR; t++) {
    media_alto = media_alto + (double)t_altos[t] / TAM_VETOR;
    media_baixo = media_baixo + (double)t_baixos[t] / TAM_VETOR;
  }
  
  periodo_alto = media_alto * periodoTiva_ns;
  periodo_baixo = media_baixo * periodoTiva_ns;
  periodo = periodo_alto + periodo_baixo;
  
  frequencia = 1000000000 / periodo;
  
  //Calcula o duty cycle
  duty_cycle = media_alto / (media_alto + media_baixo);
  /*
  if(duty_cycle >= 0.99) {
    UARTprintf("Time Out\n");
  }
  */
}

void imprime(void) {
  duty_cycle = duty_cycle * 100;
  duty_cycle_decimal = (uint32_t)((duty_cycle - (uint32_t)duty_cycle) * 100);
  //duty_cycle = (uint8_t)duty_cycle;
  
  UARTprintf("Duty cycle: %d,%d\%\%\n\n", (uint8_t)duty_cycle, (uint8_t)duty_cycle_decimal);
  UARTprintf("Periodo: %d ns\n", (uint32_t)periodo);
  UARTprintf("Frequencia: %d Hz\n", (uint32_t)frequencia);
}

void main(void){
  GPIO_Init();
  UARTInit();
  TimerInit();
  PortM_Output(BIT6);
  
  frequenciaTiva_MHz = SystemCoreClock / 1000000;
  periodoTiva_ns = (1 / frequenciaTiva_MHz) * 1000;
  
  uint32_t contagemTimeout = 0;
  uint32_t valor_ms_ns = 1000000;
  uint32_t limite_timeout = (uint32_t)round((20 * valor_ms_ns) / periodoTiva_ns);
  
  while(1){
    ativaAmostragem();
    
    if(contagemTimeout < limite_timeout) {
      if(amostragemPronta()) {
        desativaAmostragem();
        
        trataLeituras();
        
        calcula();
        
        imprime();
        
        contagemTimeout = 0;
        resetaAmostragem();
      }
      
      else {
        contagemTimeout++;
      }
    }
    
    else {
      contagemTimeout = 0;
      UARTprintf("Time Out\n");
    }
  }
} // main