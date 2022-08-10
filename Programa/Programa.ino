
//----------------------------------------------------------------Pines
// USB(0,1) PULSADOR_USUARIO(2) PULSADOR_VELOCIDAD(3) BUZZER(5) PULSADOR_CONTROL(6) LED_VEL(9) LED_CONTROL(10) BT(7,8)
// MATRIZ LEDS(4,14,15,16,A0,A1,A2,A3)
 
//------------------------------------------------------------------Librerías

//------------------------------------------------------------------Declaraciones
//CREAR VARIABLE ESTADO(en que momento de la secuencia estoy parado
const int PULSADOR_USUARIO = 2;
const int PULSADOR_VEL_MOUSE = 3;
const int PULSADOR_VEL_DISPLAY = 6;

//------------------------------------------------------------------setup

void setup() {
  
  //Entradas salidas
  pinMode(PULSADOR_USUARIO, INPUT);
  pinMode(PULSADOR_VEL_MOUSE, INPUT);
  pinMode(PULSADOR_VEL_DISPLAY, INPUT);
  //USB
  //BT
  //Timers
  //Valores iniciales

  //ISR
  attachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO), selectorGeneral, RISING);
  attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_MOUSE), selectorVelMouse, RISING);
  attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_DISPLAY), selectorVelDisplay, RISING);
}


//------------------------------------------------------------------Loop
void loop() {
  

  //Recepcion BT por RX


  //Función Mouse
  //Función Control

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
//Bandera temporal
void selectorGeneral(){
  // Depende de la rutina de los leds
}

void selectorVelMouse(){

}

void selectorVelDisplay(){
  
}

//------------------------------------------------------------------Funciones
