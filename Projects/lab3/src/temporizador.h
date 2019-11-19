#define TAM_VETOR (10)         //constante que determina o tamanho do vetor de leituras

extern uint32_t leiturasAltos[TAM_VETOR];      //armazena a quantidade de leituras altas
extern uint32_t leiturasBaixos[TAM_VETOR];     //armazena a quantidade de leituras baixas

void TIMER0A_Handler(void);
void Timer1A_Handler(void);

void TimerInit(void);

bool amostragemPronta(void);
void resetaAmostragem(void);

void ativaAmostragem(void);
void desativaAmostragem(void);