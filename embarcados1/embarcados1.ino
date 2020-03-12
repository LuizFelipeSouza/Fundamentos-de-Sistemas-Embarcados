
#include <avr/io.h>
#include <util/delay.h>

const int trigPin = 13;
const int echoPin = 12;
const int pino = 11;


int Frente(){
  return B00011000;
}

int Re(){
  return B00100100;    
}

int virarDir(){
  return B00100000; // baixo e o direito
}

int virarEsq(){
  return B00001000; // baixo e o esquerdo
}


int main(void){
  unsigned char sensor_Frente; 
  unsigned char sensor_Traz;
  unsigned long duracao = 0;
  float distancia;
  DDRB = DDRB |B00101100;
  DDRD = DDRD |B00111100;
  Serial.begin (9600);
  Serial.println("Ultrasom");
  pinMode(pino,OUTPUT);
  
  while(1){
    //PORTD = Frente();
    sensor_Frente = (PIND & 128)>> 7; 
    sensor_Traz =  (PIND & 64)>> 6;
    digitalWrite(pino,HIGH);
  /* // Trigger
   PORTB = 0B00000000;  
   delayMicroseconds(2);
   PORTB = 0B01100000;
   delayMicroseconds(10);
   PORTB = 0B00000000;
   */
   // detectar eco sem uso de biblioteca
   duracao = 0;
   while (!(PINB & 16)>> 4);
   while ((PINB & 16)>> 4)
   {
     duracao++;
   }   
   if (duracao > 0)
   {
    //Serial.println(duracao);  
    distancia= duracao * 0.0546; //calibracao com sensor
    Serial.println(distancia);        
   }
    if(distancia > 25) {
      PORTD = Frente();
      _delay_ms(10000);
      PORTD = virarEsq();
    }
    else{
      PORTD = B00000000;
      _delay_ms(1000);
    }
    if (sensor_Frente == 1){
     PORTD = Re();
     _delay_ms(2000);
     PORTD = virarEsq();
    }  
    if (sensor_Traz == 1){
        PORTD = Frente();
        _delay_ms(2000);
        PORTD = virarDir();
    }
    if (sensor_Traz == 0 && sensor_Frente == 0 ){
      PORTD = B00100100; 
    }
  }
}
