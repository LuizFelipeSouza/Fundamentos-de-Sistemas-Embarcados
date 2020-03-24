#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>

volatile unsigned long timer1_millis; // faixa de valores vai de 0 a 4.294.967.295 (2^32 - 1)
const int trigPin = 13;
const int echoPin = 12;
const int pino = 11;
const int gan = 9;

//--------------------------------------------------------
// rotina de interruoção do TIMER1
ISR(TIMER1_COMPA_vect) {
  timer1_millis++;
}

//--------------------------------------------------------
// configura e inicia o TIMER1
void inicia_millis(unsigned long f_cpu) {
  unsigned long ctc_match_overflow;
  ctc_match_overflow = ((f_cpu / 1000) / 8); // overflow do timer em 1ms

  // (limpar timer qdo cooresponder a ctc_match_overflow) | (clock divisor por 8)
  TCCR1B |= (1 << WGM12) | (1 << CS11);

  // high byte primeiro, depois low byte
  OCR1AH = (ctc_match_overflow >> 8);
  OCR1AL = ctc_match_overflow;

  // habilita compare match interrupt
  TIMSK1 |= (1 << OCIE1A);

  //habilita GLOBAL INTERRUPTS
  sei();
}

//--------------------------------------------------------
//funcao que retorna o numero de milisegundos passados
// desde que o microcontrolador foi iniciado
unsigned long nossamillis() {
  unsigned long millis_return;
  // bloca execucao
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    millis_return = timer1_millis;
  }
  return millis_return;
}

// Funcao de locomocao que retorna porta faz robo ir para frente
int Frente() {
  return 0B00011000;
}
// Funcao de locomocao que retorna porta faz robo ir para tras
int Re() {
  return 0B00100100;
}
// Funcao de locomocao que retorna porta faz robo vira para direita
int virarDir() {
  return 0B00100000; 
}
// Funcao de locomocao que retorna porta faz robo virar a esquerda
int virarEsq() {
  return 0B00001000; 
}

/**
 * Função utilizada para substituir o delay().
 * 
 * parametros:
 *  intervalo: o tempo de atraso
 *  funcao: a funçãoque será executada durante o atraso, sendo elas:
 *                    1 - Re()
 *                    2 - Frente()
 *                    3 - VirarDir()
 *                    4 - virarEsq()
 * return: void **/
 
void atraso(int intervalo, int funcao) {
  inicia_millis(16000000UL); //clock 16MHz
  unsigned long tempo_inicial = 0;
  // Configura pino io atmega para saida
  unsigned long tempo_final = nossamillis();
  while (tempo_final - tempo_inicial <= intervalo) {
    switch (funcao) {
    case 1:
      PORTD = Re();
      break;
    case 2:
      PORTD = Frente();
      break;
    case 3:
      PORTD = virarDir();
      break;
    case 4:
      PORTD = virarEsq();
      break;
    default:
      break;
    }
  }
}

int main(void){
  unsigned char sensor_Frente; 
  unsigned char sensor_Traz;
  unsigned long duracao = 0;
  float distancia;
  DDRB = DDRB |0B00101100;
  DDRD = DDRD |0B00111100;
  Serial.begin (9600);
  Serial.println("Ultrasom");
  _delay_ms(1000);
  pinMode(pino,OUTPUT);
  pinMode(gan,OUTPUT);
  while(1){
    sensor_Frente = (PIND & 128)>> 7; 
    sensor_Traz =  (PIND & 64)>> 6;
    digitalWrite(pino,HIGH);
    digitalWrite(gan,HIGH);

    // Trigger
    PORTB = 0B00001010;
    delayMicroseconds(2);
    PORTB = 0B00101010;
    delayMicroseconds(10);
    PORTB = 0B00001010; 

    // detectar eco sem uso de biblioteca
    duracao = 0;
    
    while (!(PINB & 16)>> 4);
    while ((PINB & 16)>> 4)
    {
      duracao++;
    }
    if (duracao > 0)
    { 
     distancia= duracao * 0.0546; //calibracao com sensor
     Serial.println(distancia);
    }*/
     if (sensor_Frente){
       atraso(5000,1);
       atraso(2000,3);
     }
     if (sensor_Traz){
       atraso(5000,2);
       atraso(2000,4);
     } 
     if((!sensor_Frente) && (!sensor_Traz)){
       PORTD = Frente(); 
     }
     if(distancia < 25){
       PORTD = Frente();
     }
  }
}
