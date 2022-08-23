
 //----------------------------------------------------------------Pines
 // USB(0,1) PULSADOR_USUARIO(2) PULSADOR_VELOCIDAD(3) BUZZER(5) PULSADOR_CONTROL(6) LED_VEL(9) LED_CONTROL(10) BT(7,8)
 // MATRIZ LEDS(4,14,15,16,A0,A1,A2,A3)
 int pin1=7, pin2=6, pin3=5, pin4=4, pin5=3, pin6=2; //pin1 pin2 y pin3 leds pin 4 5 6 columnas
int i=0, j=0;

//------------------------------------------------------------------Librerías

//------------------------------------------------------------------Declaraciones
//CREAR VARIABLE ESTADO(en que momento de la secuencia estoy parado


//------------------------------------------------------------------setup
// probando el git
void setup() {
  Serial.begin(9600);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A =65000;
  TCCR1B |= (1 << WGM12); //CTC
  TCCR1B |= (1 << CS12); 
   //prescaler
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
//Entradas salidas
//USB
//BT
//Timers
//Valores iniciales

}


//------------------------------------------------------------------Loop
void loop() {
  

//Recepcion BT por RX


//Función Mouse
//Función COntrol

/*switch (Modo){
  case 1:
  //Mouse a 1 capas
  break;
  case 2:
  //Mouse 2 capa
  break;
  
  case 3:
  //PIctogramas por grupo
  break;
  
  case 4:
  //pictograma por unidad
  break;
  case 5:
  //POSIBLE 5TO MODO DE CONFIGURACIÓN
  break;}*/


}


//------------------------------------------------------------------ISR
ISR(TIMER1_COMPA_vect){
  if(i==0){
    digitalWrite(pin4, LOW);
    digitalWrite(pin5, HIGH);
    digitalWrite(pin6, HIGH);
    switch (j){
      case 0:
      digitalWrite(pin3,LOW);
      digitalWrite(pin1,HIGH);
      j++;
      break;
      case 1:
      digitalWrite(pin1,LOW);
      digitalWrite(pin2,HIGH);
      j++;
      break;
      case 2:
      digitalWrite(pin2,LOW);
      digitalWrite(pin3,HIGH);
      j=0;
      i++;
      break;
      }
      
  }
  else
    {
    digitalWrite(pin4, HIGH);
    digitalWrite(pin5, LOW);
    digitalWrite(pin6, HIGH);
    switch (j){
      case 0:
      digitalWrite(pin3,LOW);
      digitalWrite(pin1,HIGH);
      j++;
      break;
      case 1:
      digitalWrite(pin1,LOW);
      digitalWrite(pin2,HIGH);
      j++;
      break;
      case 2:
      digitalWrite(pin2,LOW);
      digitalWrite(pin3,HIGH);
      j=0;
      i=0;
      break;
      }
     
  }

}
//Bandera temporal




//------------------------------------------------------------------Funciones
