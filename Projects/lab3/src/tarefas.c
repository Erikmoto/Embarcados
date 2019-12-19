#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "system_tm4c1294.h" // CMSIS-Core
#include "cmsis_os2.h" // CMSIS-RTOS
#include "driverlib/uart.h"
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "UARTInit.h"
#include "comandos_elevador.h"
#include "respostas_elevador.h"

#define FLAG_MENSAGEM 0x0001
#define FLAG_ELEVADOR_1 0x0010
#define FLAG_ELEVADOR_2 0x0100
#define FLAG_ELEVADOR_3 0x1000

typedef struct {
  char comando[6];
  char estado;
  bool requisicoes[16];
  int posicao;
  uint8_t andar;
} Elevador;

osThreadId_t elevador_1_id, elevador_2_id, elevador_3_id, mensagens_id;
osMutexId_t mutex_console_id, mutex_uart_id;
osMemoryPoolId_t elevador_1_pool_id, elevador_2_pool_id, elevador_3_pool_id;

Elevador *E1, *E2, *E3;

void UART0_Handler(void) {
  uint32_t uart_status = UARTIntStatus(UART0_BASE, true);
  UARTIntClear(UART0_BASE, uart_status);
  
  osThreadFlagsSet(mensagens_id, 0x0001);
}

void escreve_console(char* texto) {
  printf("%s", texto);
}

void thread_elevador_1(void *arg){
  char comando[6];
  char estado;
  bool requisicoes[16];
  uint8_t botao, andar, andar_requisitado;
  int i;
  
  reiniciar_elevador('e');
  E1->estado = 'p';
  E1->andar = 0;
  E1->posicao = 0;
  
  osThreadFlagsClear(0x0001);
  osThreadFlagsSet(mensagens_id, FLAG_ELEVADOR_1);
  
  while(1){
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    
    for(i = 0; i < 6; i++) {
      comando[i] = E1->comando[i];
    }
    
    estado = E1->estado;
    andar = E1->andar;
    
    for(i = 0; i < 16; i++) {
      requisicoes[i] = E1->requisicoes[i];
    }
    
    osThreadFlagsClear(0x0001);
    osThreadFlagsSet(mensagens_id, FLAG_ELEVADOR_1);
    
    if(comando[0] == 'A') {
      //printf("\nE1: Portas Abertas\n");
      E1->requisicoes[andar] = false;
      
      estado = elevador_aberto('e', estado, requisicoes, andar);
    }
    
    else if(comando[0] == 'F') {
      //printf("\nE1: Portas Fechadas, Estado: %c\n", estado);
      elevador_fechado('e', estado);
    }
    
    else if(comando[0] == 'E') {
      andar_requisitado = (comando[1] - 0x30) * 10;
      andar_requisitado += (comando[2] - 0x30);
      
      //printf("\nE1: Requisição Andar %d\n", andar_requisitado);
      
      estado = botao_externo_pressionado('e', estado, comando[3], andar, andar_requisitado);
      
      E1->requisicoes[andar_requisitado] = true;
      
      if(estado != 'p') {
        fechar_elevador('e');
      }
    }
    
    else if(comando[0] == 'I') {
      botao = comando[1] - 'a';
      
      //printf("I");
      
      ligar_luz('e', comando[1]);
      
      estado = botao_interno_pressionado('e', estado, botao, andar);
      
      E1->requisicoes[botao] = true;
      
      if(estado == 'p') {
        E1->requisicoes[botao] = true;
        desligar_luz('e', comando[1]);
      }
      
      else {
        fechar_elevador('e');
      }
    }
    
    else if(comando[0] >= '0' && comando[0] <= '9') {
      andar = comando[0] - 0x30;
      
      if(comando[1] != 0x0D) {
        andar = andar * 10;
        andar += (comando[1] - 0x30);
      }
      
      //printf("\nE1: Andar %d\n", andar);
      
      estado = andar_atingido('e', estado, requisicoes, andar);
      /*
      E1->posicao = -1;
      
      posicao_elevador('e');
      
      osThreadFlagsWait(0x0010, osFlagsWaitAny, osWaitForever);
      osThreadFlagsClear(0x0010);
      
      int posicao_minima = andar * 5000, posicao_maxima = andar * 5000 + 100;
      
      while(E1->posicao < posicao_minima || E1->posicao > posicao_maxima) {
        if(E1->posicao < posicao_minima) {
          subir_elevador('e');
        }
        
        else if(E1->posicao > posicao_maxima) {
          descer_elevador('e');
        }
        
        parar_elevador('e');
        E1->posicao = -1;
        
        osThreadFlagsWait(0x0010, osFlagsWaitAny, osWaitForever);
        osThreadFlagsClear(0x0010);
      }
      
      abrir_elevador('e');
      */
    }
    
    E1->estado = estado;
    E1->andar = andar;
  } // while
} // thread1

void thread_elevador_2(void *arg){
  char comando[6];
  char estado;
  bool requisicoes[16];
  uint8_t botao, andar, andar_requisitado;
  int i;
  
  reiniciar_elevador('c');
  E2->estado = 'p';
  E2->andar = 0;
  E2->posicao = 0;
  
  osThreadFlagsClear(0x0001);
  osThreadFlagsSet(mensagens_id, FLAG_ELEVADOR_2);
  
  while(1){
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    
    for(i = 0; i < 6; i++) {
      comando[i] = E2->comando[i];
    }
    
    estado = E2->estado;
    andar = E2->andar;
    
    for(i = 0; i < 16; i++) {
      requisicoes[i] = E2->requisicoes[i];
    }
    
    osThreadFlagsClear(0x0001);
    osThreadFlagsSet(mensagens_id, FLAG_ELEVADOR_2);
    
    if(comando[0] == 'A') {
      E2->requisicoes[andar] = false;
      
      estado = elevador_aberto('c', estado, requisicoes, andar);
    }
    
    else if(comando[0] == 'F') {
      elevador_fechado('c', estado);
    }
    
    else if(comando[0] == 'E') {
      andar_requisitado = (comando[1] - 0x30) * 10;
      andar_requisitado += (comando[2] - 0x30);
      
      estado = botao_externo_pressionado('c', estado, comando[3], andar, andar_requisitado);
      
      E2->requisicoes[andar_requisitado] = true;
      
      if(estado != 'p') {
        fechar_elevador('c');
      }
    }
    
    else if(comando[0] == 'I') {
      botao = comando[1] - 'a';
      
      ligar_luz('c', comando[1]);
      
      estado = botao_interno_pressionado('c', estado, botao, andar);
      
      E2->requisicoes[botao] = true;
      
      if(estado == 'p') {
        E2->requisicoes[botao] = true;
        desligar_luz('c', comando[1]);
      }
      
      else {
        fechar_elevador('c');
      }
    }
    
    else if(comando[0] >= '0' && comando[0] <= '9') {
      andar = comando[0] - 0x30;
      
      if(comando[1] != 0x0D) {
        andar = andar * 10;
        andar += (comando[1] - 0x30);
      }
      
      estado = andar_atingido('c', estado, requisicoes, andar);
    }
    
    E2->estado = estado;
    E2->andar = andar;
  }
} // thread2

void thread_elevador_3(void *arg){
  char comando[6];
  char estado;
  bool requisicoes[16];
  uint8_t botao, andar, andar_requisitado;
  int i;
  
  reiniciar_elevador('d');
  E3->estado = 'p';
  E3->andar = 0;
  E3->posicao = 0;
  
  osThreadFlagsClear(0x0001);
  osThreadFlagsSet(mensagens_id, FLAG_ELEVADOR_3);
  
  while(1){
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    
    for(i = 0; i < 6; i++) {
      comando[i] = E3->comando[i];
    }
    
    estado = E3->estado;
    andar = E3->andar;
    
    for(i = 0; i < 16; i++) {
      requisicoes[i] = E3->requisicoes[i];
    }
    
    osThreadFlagsClear(0x0001);
    osThreadFlagsSet(mensagens_id, FLAG_ELEVADOR_3);
    
    if(comando[0] == 'A') {
      E3->requisicoes[andar] = false;
      
      estado = elevador_aberto('d', estado, requisicoes, andar);
    }
    
    else if(comando[0] == 'F') {
      elevador_fechado('d', estado);
    }
    
    else if(comando[0] == 'E') {
      andar_requisitado = (comando[1] - 0x30) * 10;
      andar_requisitado += (comando[2] - 0x30);
      
      estado = botao_externo_pressionado('d', estado, comando[3], andar, andar_requisitado);
      
      E3->requisicoes[andar_requisitado] = true;
      
      if(estado != 'p') {
        fechar_elevador('d');
      }
    }
    
    else if(comando[0] == 'I') {
      botao = comando[1] - 'a';
      
      ligar_luz('d', comando[1]);
      
      estado = botao_interno_pressionado('d', estado, botao, andar);
      
      E3->requisicoes[botao] = true;
      
      if(estado == 'p') {
        E3->requisicoes[botao] = false;
        desligar_luz('d', comando[1]);
      }
      
      else {
        fechar_elevador('d');
      }
    }
    
    else if(comando[0] >= '0' && comando[0] <= '9') {
      andar = comando[0] - 0x30;
      
      if(comando[1] != 0x0D) {
        andar = andar * 10;
        andar += (comando[1] - 0x30);
      }
      
      estado = andar_atingido('d', estado, requisicoes, andar);
    }
    
    E3->estado = estado;
    E3->andar = andar;
  }
} // thread3

void thread_mensagens(void *arg) {
  char caractere;
  int i, posicao;
  
  while(1) {
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    
    while(UARTCharsAvail(UART0_BASE)) {
      caractere = UARTCharGet(UART0_BASE);
      
      for(i = 0; i < 6; i++) {
        E1->comando[i] = 0x0D;
        E2->comando[i] = 0x0D;
        E3->comando[i] = 0x0D;
      }
      
      i = 0;
      
      if(caractere == 'e') {
        while(caractere != 0x0D && caractere != 0x0A) {
          caractere = UARTCharGet(UART0_BASE);
          E1->comando[i] = caractere;
          i++;
        }
        
        //printf("e%s", E1->comando);
        
        osThreadFlagsSet(elevador_1_id, 0x0001);
        osThreadFlagsWait(FLAG_ELEVADOR_1, osFlagsWaitAny, osWaitForever);
        osThreadFlagsClear(FLAG_ELEVADOR_1);
      }
      
      else if(caractere == 'c') {
        while(caractere != 0x0D && caractere != 0x0A) {
          caractere = UARTCharGet(UART0_BASE);
          E2->comando[i] = caractere;
          i++;
        }
        
        //printf("c%s", E2->comando);
        
        osThreadFlagsSet(elevador_2_id, 0x0001);
        osThreadFlagsWait(FLAG_ELEVADOR_2, osFlagsWaitAny, osWaitForever);
        osThreadFlagsClear(FLAG_ELEVADOR_2);
      }
      
      else if(caractere == 'd') {
        while(caractere != 0x0D && caractere != 0x0A) {
          caractere = UARTCharGet(UART0_BASE);
          E3->comando[i] = caractere;
          i++;
        }
        
        //printf("d%s", E3->comando);
        
        osThreadFlagsSet(elevador_3_id, 0x0001);
        osThreadFlagsWait(FLAG_ELEVADOR_3, osFlagsWaitAny, osWaitForever);
        osThreadFlagsClear(FLAG_ELEVADOR_3);
      }
      
      else if(caractere >= '0' && caractere <= '9') {
        /*
        i = 4;
        posicao = 0;
        
        if(E1->posicao == -1) {
          while(caractere != 0x0D && caractere != 0x0A) {
            posicao += (caractere - 0x30) * pow(10, i);
            
            caractere = UARTCharGet(UART0_BASE);
            i--;
          }
          
          if(i > 0) {
            posicao = posicao / pow(10, i);
          }
          
          E1->posicao = posicao;
          osThreadFlagsSet(elevador_1_id, 0x0010);
        }
        */
      }
      
      else {
        //printf("\n");
      }
    }
    
    osThreadFlagsClear(0x0001);
  }
}

void main(void){
  UARTInit();
  
  UARTConfigSetExpClk(UART0_BASE, SystemCoreClock, 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
  UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
  UARTIntDisable(UART0_BASE, 0xFFFFFFFF);
  UARTIntEnable(UART0_BASE, UART_INT_RX);
  IntEnable(INT_UART0);
  UARTEnable(UART0_BASE);
  
  //UARTIntRegister(UART0_BASE, UART0_Handler);
  //UARTStdioConfig(0, 115200, SystemCoreClock);
  
  osKernelInitialize();
  
  elevador_1_id = osThreadNew(thread_elevador_1, NULL, NULL);
  while(elevador_1_id == NULL) {
    elevador_1_id = osThreadNew(thread_elevador_1, NULL, NULL);
  }
  
  elevador_2_id = osThreadNew(thread_elevador_2, NULL, NULL);
  while(elevador_2_id == NULL) {
    elevador_2_id = osThreadNew(thread_elevador_2, NULL, NULL);
  }
  
  elevador_3_id = osThreadNew(thread_elevador_3, NULL, NULL);
  while(elevador_3_id == NULL) {
    elevador_3_id = osThreadNew(thread_elevador_3, NULL, NULL);
  }
  
  mensagens_id = osThreadNew(thread_mensagens, NULL, NULL);
  while(mensagens_id == NULL) {
    mensagens_id = osThreadNew(thread_mensagens, NULL, NULL);
  }
  
  elevador_1_pool_id = osMemoryPoolNew(1, sizeof(Elevador), NULL);
  elevador_2_pool_id = osMemoryPoolNew(1, sizeof(Elevador), NULL);
  elevador_3_pool_id = osMemoryPoolNew(1, sizeof(Elevador), NULL);
  
  int i;
  
  E1 = (Elevador*)osMemoryPoolAlloc(elevador_1_pool_id, osWaitForever);
  
  for(i = 0; i < 16; i++) {
    E1->requisicoes[i] = false;
  }
  
  E2 = (Elevador*)osMemoryPoolAlloc(elevador_2_pool_id, osWaitForever);
  
  for(i = 0; i < 16; i++) {
    E2->requisicoes[i] = false;
  }
  
  E3 = (Elevador*)osMemoryPoolAlloc(elevador_3_pool_id, osWaitForever);
  
  for(i = 0; i < 16; i++) {
    E3->requisicoes[i] = false;
  }
  
  //mutex_console_id = osMutexNew(NULL);
  //mutex_uart_id = osMutexNew(NULL);
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
