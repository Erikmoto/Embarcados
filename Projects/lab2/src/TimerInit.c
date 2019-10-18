#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"

#include "system_TM4C1294.h"

#define TAM_VETOR (24000)         //constante que determina o tamanho do vetor de leituras
bool subida = 0;                 // Armazena a leitura do pino de entrada
uint16_t t_altos[TAM_VETOR];      //armazena a quantidade de leituras altas
uint16_t t_baixos[TAM_VETOR];     //armazena a quantidade de leituras baixas
uint16_t count;                 //Conta as vezes que a interrupção ocorreu 
uint16_t t;                     //Indice utilizado para percorrer os vetores
double media_alto;            //Armazena a media dos tempos altos
double media_baixo;           //Armazena a media dos tempos baixos
double duty_cycle_1;            //Armazena o calculo do duty cycle
void calcula()
{
  //Zera as variaveis
  t=0;
  media_alto = 0;
  media_baixo = 0;
  
  //Soma todos os valores dos vetores
  while(t<=TAM_VETOR)
  {
    media_alto = media_alto + t_altos[t];
    media_baixo = media_baixo + t_baixos[t];
    t++;
  }
  
  //Divide pela quantidade de leituras para obter a media
  media_alto = media_alto/TAM_VETOR;
  media_baixo = media_baixo/TAM_VETOR;
  
  //Calcula o duty cycle
  duty_cycle_1 = media_alto/(media_alto+media_baixo);
}

void TIMER0A_Handler() {
  TimerLoadSet(TIMER0_BASE, TIMER_A, 0);
  TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
  
  subida = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0);// Armazena a leitura do pino de entrada
  
  //Salva o tempo desde a ultima borda no vetor apropriado
  if(subida)
  {
    t_baixos[count/2] = 9; //Substituir 9 pela leitura do timer
  }
  else
  {
    t_altos[count/2] = 9; //Substituir 9 pela leitura do timer
  }
  
  //Se preencheu o vetor, zera count e chama a função de cálculo
  if((count/2) >= TAM_VETOR)
  {
    count = -1;
    calcula();
  }
  
  //Aumenta 1 no count. Com exceção quando a primeira borda seja uma descida
  if((count == 0)&&(!subida))count ++;  
}

void TimerInit(void){
  // Enable the GPIO Peripheral used by the Timer 0.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

  // Enable Timer 0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
  
  GPIOPinConfigure(GPIO_PD0_T0CCP0);
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
  //GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0);
  //GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP));
  
  //TimerControlLevel(TIMER0_BASE, TIMER_A, true);
  //TimerPrescaleSet(TIMER0_BASE, TIMER_A, 10);
  TimerLoadSet(TIMER0_BASE, TIMER_A, 0);
  //TimerMatchSet(TIMER0_BASE, TIMER_A, pulseTimeWidth);
  TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
  
  TimerIntRegister(TIMER0_BASE, TIMER_A, TIMER0A_Handler);
  TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerEnable(TIMER0_BASE, TIMER_A);
} // TimerInit