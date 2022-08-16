int pin1=10, 
    pin2=9, 
    pin3=8, 
    pin4=7, 
    pin5=6, 
    pin6=5, 
    pin7=4, 
    pin8=3, 
    pin9=2;

int pines[]={pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9};
int cont = 0;
    
int pines[]={pin1, pin2, pin3, pin4};
int cont = 0;

void setup() {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  pinMode(pin8, OUTPUT);
  pinMode(pin9, OUTPUT);
  noInterrupts();
  noInterrupts();          
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 31250;           //prescaler 256 a 500ms la interrupción
  TCCR1B |= (1 << WGM12);  
  TCCR1B |= (1 << CS12);   
  TIMSK1 |= (1 << OCIE1A);  
  interrupts();             


  }
ISR(TIMER1_COMPA_vect){
  if(cont>0){
  digitalWrite(pines[cont], HIGH);
  digitalWrite(pines[cont-1], LOW);
  cont++;
  }
  else{
    digitalWrite(pines[8], LOW);
    digitalWrite(pines[0], HIGH);
    cont++;}
    if(cont==9)
    cont=0;
  
  }
  
  
void loop() {

}

   
