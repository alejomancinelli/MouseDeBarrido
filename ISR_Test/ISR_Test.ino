
//----------------------------------------------------------------Pines
// USB(0,1) PULSADOR_USUARIO(2) PULSADOR_VELOCIDAD(3) BUZZER(5) PULSADOR_CONTROL(6) LED_VEL(9) LED_CONTROL(10) BT(7,8)
// MATRIZ LEDS(4,14,15,16,A0,A1,A2,A3)
 
//------------------------------------------------------------------Librerías

//------------------------------------------------------------------Declaraciones
//CREAR VARIABLE ESTADO(en que momento de la secuencia estoy parado
const int PULSADOR_USUARIO = 2;
const int ledPin = 13;
volatile bool state = 0;

//------------------------------------------------------------------setup

void setup() {
  
//Entradas salidas
pinMode(PULSADOR_USUARIO, INPUT);
pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, state);
//USB
//BT
//Timers
//Valores iniciales
//ISR
attachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO), pulsadorSelector, RISING);

}


//------------------------------------------------------------------Loop
void loop() {
  
  digitalWrite(ledPin, state);
//Recepcion BT por RX


//Función Mouse
//Función COntrol


}


//------------------------------------------------------------------ISR
//Bandera temporal
void pulsadorSelector(){
  state = !state;
  
}



//------------------------------------------------------------------Funciones
