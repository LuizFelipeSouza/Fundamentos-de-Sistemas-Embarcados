
const int trigPin = 13;
const int echoPin = 12;


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


/*int Traz(int rum){
  if(rum){
      //traz direita positivo 
      PORTD = B00010100;
      _delay_ms(3000);
      rum = 0;
    }
  else{
      PORTD = B00000000;
      _delay_ms(1000);
      rum = 1;
      Traz(rum);
    }
  Traz(rum);
}*/

int main(void){
  unsigned char sensor_pino_7; 
  unsigned char sensor_pino_6;
  unsigned long duracao = 0;
  float distancia;
  DDRB = B11101111;
  DDRD = DDRD |B00111100;
  Serial.begin (9600);
  Serial.println("Ultrasom");
  
  while(1){
    //PORTD = Frente();
    sensor_pino_7 =   (PIND & 128)>> 7; 
    sensor_pino_6 =   (PIND & 64)>> 6;
    
    // Trigger
   PORTB = 0B00000000;  
   delayMicroseconds(2);
   PORTB = 0B00100000;
   delayMicroseconds(10);
   PORTB = 0B00000000;
   
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
    if(distancia < 2){
      PORTD = Frente();
    }
    else{
      PORTD = B00000000;
    }
    if (sensor_pino_7 == 1){
     PORTD = Re();
    }  
    if (sensor_pino_6 == 1){
       
        PORTD = Frente();
    }
    if (sensor_pino_6 == 0 && sensor_pino_7 == 0 ){
      PORTD = B00111100; 
    }
    
    /*if(){
      //frente direita negativo
      PORTD = B00101000;
      _delay_ms(3000);
      rum = 0;
    }
    if(){
      //traz direita positivo 
      PORTD = B00010100;
      _delay_ms(3000);
      rum = 0;
    }
    if(){
      //esquerda frente
      PORTD = B00100000;
      _delay_ms(1000);
      rum = 0;
    }
    if(){
      //direita frente
      PORTD = B00001000;
      _delay_ms(1000);
      rum = 0;
    }
    if(){
      //esquerda re
      PORTD = B00010000;
      _delay_ms(1000);
      rum = 0;
    }
    
    if(){
      //direita re
      PORTD = B00000100;
      _delay_ms(1000);
      rum = 0;
    }
    else{
      PORTD = B00000000;
      _delay_ms(2000);
      rum = 1;
    }*/
  }
}
