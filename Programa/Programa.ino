 //----------------------------------------------------------------Pines
 // USB(0,1) PULSADOR_USUARIO(2) PULSADOR_VELOCIDAD(3) BUZZER(5) PULSADOR_CONTROL(6) LED_VEL(9) LED_CONTROL(10) BT(7,8)
 // MATRIZ LEDS(4,14,15,16,A0,A1,A2,A3)
int VCC_LED_1=7, VCC_LED_2=6, VCC_LED_3=5, GND_LED_1=4, GND_LED_2=3, GND_LED_3=2; //VCC_LED_1 VCC_LED_2 y VCC_LED_3 leds pin 4 5 6 columnas
int MATRIZ_LED[2][3] = {{VCC_LED_1, VCC_LED_2, VCC_LED_3}, {GND_LED_1, GND_LED_2, GND_LED_3}};
int i=0, j=0;
//------------------------------------------------------------------Librerías

//------------------------------------------------------------------Declaraciones
//CREAR VARIABLE ESTADO(en que momento de la secuencia estoy parado
const int PULSADOR_USUARIO = 2;
const int PULSADOR_VEL_MOUSE = 3;
const int PULSADOR_VEL_DISPLAY = 6;

//------------------------------------------------------------------setup
// probando el git
void setup() {
  Serial.begin(9600);
  pinMode(VCC_LED_1, OUTPUT);
  pinMode(VCC_LED_2, OUTPUT);
  pinMode(VCC_LED_3, OUTPUT);
  pinMode(GND_LED_1, OUTPUT);
  pinMode(GND_LED_2, OUTPUT);
  pinMode(GND_LED_3, OUTPUT);
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
digitalWrite(GND_LED_1, HIGH);
digitalWrite(GND_LED_2, HIGH);
digitalWrite(GND_LED_3, HIGH);

attachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO), selectorGeneral, RISING);
attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_MOUSE), selectorVelMouse, RISING);
attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_DISPLAY), selectorVelDisplay, RISING);
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
      break;
    case default:
      break;
  }

}


//------------------------------------------------------------------ISR
ISR(TIMER1_COMPA_vect){
  digitalWrite(MATRIZ_LED[1][i], HIGH);
  digitalWrite(MATRIZ_LED[0][j], LOW);
  j++;
  if(j > 2){
    j = 0;
    i == 2 ? i = 0 : i++;
  }
  digitalWrite(MATRIZ_LED[1][i], LOW);
  digitalWrite(MATRIZ_LED[0][j], HIGH);

}
//Bandera temporal
void selectorGeneral(){
  // Depende de la rutina de los leds
}

void selectorVelMouse(){


}

void selectorVelDisplay(){
  
}
//------------------------------------------------------------------Funciones
