//---------------------------------------------------------------- Pines
// USB(0,1) PULSADOR_USUARIO(2) BUZZER(5) LED_CONTROL(10) BT(7,8)
// MATRIZ LEDS(A0, A1, A2, 16, 14, 15) multiplexados

//---------------------------------------------------------------- Librerías
#include <Mouse.h>
#include <Keyboard.h>
#include <SoftwareSerial.h>
SoftwareSerial BT(8, 7);  //RX TX (evaluar pin para rx  tx)

//---------------------------------------------------------------- Declaraciones
#define MAX_VEL_LEDS 3

const int PULSADOR_USUARIO = 2,
          BLUETOOTH_TX = 7,
          BLUETOOTH_RX = 8,
          VCC_LED_1 = A0, 
          VCC_LED_2 = A1, 
          VCC_LED_3 = A2, 
          GND_LED_1 = 16, 
          GND_LED_2 = 14, 
          GND_LED_3 = 15; 
          
const int MATRIZ_LED[2][3] = {{VCC_LED_1, VCC_LED_2, VCC_LED_3}, 
                              {GND_LED_1, GND_LED_2, GND_LED_3}};

bool arrayLedsEncendidos[3][3] = {{0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

// Por como tratamos a la matriz, las filas se encuentras a GND y las columnas a VCC, pero se podría cambiar
int matrizFila = 0, matrizColumna = 0; 
bool userInput = 0;

const int timeThreshold = 200;
long startTimePulsador = 0;

// Calculo OCR = t * (f / PS)
// OCR1A, TCCR1B -> CS10
const int TIMER1_INTERRUPTS[MAX_VEL_LEDS][2] = {{31250, 0},    // 0.5 seg, PS: 256   OCR: 31250 
                                                {65000, 0},    // 1 seg,   PS: 256   OCR: 65000 
                                                {31250, 1}};   // 2 seg,   PS: 1024  OCR: 31250 
int timer1InterruptIndex = 1;

bool interruptFlagTimer3 = 0;

int  modo = 1, vel_mouse = 1, range = 5, mov = range * vel_mouse, comando; //comando almacena datos bluetooth

//---------------------------------------------------------------- Setup
void setup() {
  Serial.begin(9600);
  // Keyboard.begin();
  BT.begin(9600);
  pinMode(VCC_LED_1, OUTPUT);
  pinMode(VCC_LED_2, OUTPUT);
  pinMode(VCC_LED_3, OUTPUT);
  pinMode(GND_LED_1, OUTPUT);
  pinMode(GND_LED_2, OUTPUT);
  pinMode(GND_LED_3, OUTPUT);
  noInterrupts();
  //Timers
  setupTimer1();
  setupTimer3();
  // Interrupciones de pulsador por rising edge
  attachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO), selectorGeneral, RISING);
  //  attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_MOUSE), selectorVelMouse, RISING);
  attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_DISPLAY), selectorVelDisplay, RISING);
  interrupts();
  //USB
  //BT
  //Valores iniciales
  digitalWrite(GND_LED_1, HIGH);
  digitalWrite(GND_LED_2, HIGH);
  digitalWrite(GND_LED_3, HIGH);
}


//---------------------------------------------------------------- Loop
void loop() {
  if (userInput) {
    mouse_control();
  }
  Bluetooth();

  //Recepcion BT por RX
  //Función Mouse

}

//---------------------------------------------------------------- ISR
ISR(TIMER1_COMPA_vect) {
  switch (modo) {
    case 1:
      modo1();
      break;
      /*case 2:
          MODO2();
          break;
        case 3:
          MODO3();
          break;*/
  }
}

ISR(TIMER3_COMPA_vect) {
  Serial.println("Timer 3 interrupt");
  interruptFlagTimer3 = 1;
}

//Bandera temporal
void selectorGeneral(){
  if(millis() - startTimePulsador > timeThreshold){
    startTimePulsador = millis();
    TCNT1 = 0; // Se limpia el contador del timer 1 
    Serial.println("Pulsador de usuario");
    if(!userInput){
      TIMSK1 &= ~(1 << OCIE1A);   // Output compare Timer1 A Interrupt Disable
      TIMSK3 |= (1 << OCIE3A);    // Output compare Timer3 A Interrupt Enable
      userInput = !userInput; 
    }
    else{
      TIMSK1 |= (1 << OCIE1A);    // Output compare Timer1 A Interrupt Enable
      TIMSK3 &= ~(1 << OCIE3A);   // Output compare Timer3 A Interrupt Disable 
      userInput = !userInput;
    }
  }
}

/*void selectorVelMouse(){
  Serial.print("velocidad del mouse: ");
  Serial.println(vel_mouse);
  vel_mouse==3 ? vel_mouse=1 : vel_mouse++;
  mov= vel_mouse*range;
  }*/

// Este después se borra, se configura por BT
void selectorVelDisplay() {
  Serial.println("Velocidad de display");
  noInterrupts();
  timer1InterruptIndex == 2 ? timer1InterruptIndex = 0 : timer1InterruptIndex++;
  Serial.print("Index: ");
  Serial.println(timer1InterruptIndex);
  OCR1A = TIMER1_INTERRUPTS[timer1InterruptIndex][0];
  TCCR1B &= ~(1 << CS10);
  TCCR1B |= (TIMER1_INTERRUPTS[timer1InterruptIndex][3] << CS10);
  interrupts();
}

//---------------------------------------------------------------- Funciones
void setupTimer1(){
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 65000;
  TCCR1B |= (1 << WGM12);   //CTC
  TCCR1B |= (1 << CS12);    // Prescaler 256
  TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable
}

void setupTimer3(){
  // Ajustar para el mov del mouse
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;
  OCR3A = 6500;
  TCCR3B |= (1 << WGM32);   //CTC
  TCCR3B |= (1 << CS32);    // Prescaler 256
}


int estado() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (arrayLedsEncendidos[i][j]) {
        return (i * 3 + j);
      }
    }
  }
  return -1;
}

void modo1() {
  digitalWrite(MATRIZ_LED[1][matrizFila], HIGH);
  digitalWrite(MATRIZ_LED[0][matrizColumna], LOW);
  arrayLedsEncendidos[matrizFila][matrizColumna] = 0;
  matrizColumna++;
  if(matrizColumna > 2){
    matrizColumna = 0;
    matrizFila == 2 ? matrizFila = 0 : matrizFila++;
  }
  digitalWrite(MATRIZ_LED[1][matrizFila], LOW);
  digitalWrite(MATRIZ_LED[0][matrizColumna], HIGH);
  arrayLedsEncendidos[matrizFila][matrizColumna] = 1;
}

void mouse_control() {
  switch (estado()) {
    //    case 0:
    //      Keyboard.press(KEY_ESC); //tecla escape posible necesidad de pequeña espera
    //      Keyboard.release(KEY_ESC);
    //      break;
    case 1:
      if(interruptFlagTimer3){
        interruptFlagTimer3 = 0;
        Serial.println("Mouse arriba"); //cambiar 20 por mov
        Mouse.move(0, (-mov), 0); //flecha arriba    10=range varias experimentalmente
      }
      break;
    // case 2:

    // break;
    case 3:
    if(interruptFlagTimer3){
        interruptFlagTimer3 = 0;
        Serial.println("Mouse izquierda");
        Mouse.move(-(mov), 0, 0); //flecha izquierda
      }
      break;
    //    case 4:
    //      modo++;
    //      break;
    case 5:
      if(interruptFlagTimer3){
        interruptFlagTimer3 = 0;
        Serial.println("Mouse derecha");
        Mouse.move((mov), 0, 0); //flecha derecha
      }
      break;
    case 6:
      TIMSK1 |= (1 << OCIE1A);    // Output compare Timer1 A Interrupt Enable
      TIMSK3 &= ~(1 << OCIE3A);   // Output compare Timer3 A Interrupt Disable 
      userInput = !userInput;
      Mouse.click(MOUSE_LEFT);
      break;
    case 7:
      if(interruptFlagTimer3){
        interruptFlagTimer3 = 0;
        Serial.println("Mouse abajo");
        Mouse.move(0, (mov), 0); //flecha abajo
      }
      break;
    case 8:
      TIMSK1 |= (1 << OCIE1A);    // Output compare Timer1 A Interrupt Enable
      TIMSK3 &= ~(1 << OCIE3A);   // Output compare Timer3 A Interrupt Disable 
      userInput = !userInput;
      Mouse.click(MOUSE_RIGHT); //click derecho
      break;
    default:
      break;
  }
}

void Bluetooth () { // a=arriba, b=below, d=derecha, i=izquierda, y=click izquierdo, z=click derecho
  if (BT.available() > 0) {    // lee el bluetooth y almacena en serial
    comando = BT.read();
    switch (comando) {
      case 'a':
        Mouse.move(0, -mov, 0);
        break;
      case 'b':
        Mouse.move(0, mov, 0);
        break;
      case 'd':
        Mouse.move(mov, 0, 0);
        break;
      case 'i':
        Mouse.move(-mov, 0, 0);
        break;
      case 'e':
        Keyboard.press(KEY_ESC); //tecla escape posible necesidad de pequeña espera
        Keyboard.release(KEY_ESC);
        break;        

      case 'y':
        Mouse.click(MOUSE_LEFT);
        break;
      case 'z':
        Mouse.click(MOUSE_RIGHT);
        break;

        //agregar funciones de configuración
    }
  }
}
