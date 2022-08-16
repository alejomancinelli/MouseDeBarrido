
 //----------------------------------------------------------------Pines
 // USB(0,1) PULSADOR_USUARIO(2) PULSADOR_VELOCIDAD(3) BUZZER(5) PULSADOR_CONTROL(6) LED_VEL(9) LED_CONTROL(10) BT(7,8)
 // MATRIZ LEDS(4,14,15,16,A0,A1,A2,A3)

 
//------------------------------------------------------------------Librerías

//------------------------------------------------------------------Declaraciones
//CREAR VARIABLE ESTADO(en que momento de la secuencia estoy parado


//------------------------------------------------------------------setup

void setup() {
  
//Entradas salidas
//USB
//BT
noInterrupts();
TCCR1A = 0;
TCCR1B = 0;
TCNT1 = 0;
OCR1A =31250;
TCCR1B |= (1 << WGM12); //CTC
TCCR1B |= (1 << CS12);      //prescaler
TIMSK1 |= ()1 << OCIE1A);
interrupts();
//Valores iniciales
}
//------------------------------------------------------------------Loop
void loop() {
  

//Recepcion BT por RX


//Función Mouse
//Función COntrol

switch (Modo){
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
  break;}


}


//------------------------------------------------------------------ISR
ISR(TIMER1_COMPA_vect){
  
  }
//Bandera temporal




//------------------------------------------------------------------Funciones
