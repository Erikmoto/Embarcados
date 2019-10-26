#define TAM_VETOR (12)         //constante que determina o tamanho do vetor de leituras

extern uint32_t t_altos[TAM_VETOR];      //armazena a quantidade de leituras altas
extern uint32_t t_baixos[TAM_VETOR];     //armazena a quantidade de leituras baixas

void TIMER0A_Handler(void);
void Timer1A_Handler(void);

void TimerInit(void);

uint32_t *getTemposAltos(void);
uint32_t *getTemposBaixos(void);

bool amostragemPronta(void);
void resetaAmostragemPronta(void);

void ativaAmostragem(void);
void desativaAmostragem(void);