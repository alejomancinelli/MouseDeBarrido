"# MouseDeBarrido" 
El Dispositivo cuenta con 9 leds que se encenderan en 3 modos distintos.
Modo 1: Mouse de barrido de una Capa
          Este Modo enciende Leds en la secuencia 7,8,9,4,6,1,2,3, (Posición en Referencia a Teclado Numerico de PC) 
                              1 Click Izq.
                             3 Click Derecho
                             7 Caracter de Escape
                             9 Arrastre
Modo 2: Mouse de barrido de 2 capas
          Capa 1: Este Modo enciende Leds en la secuencia 8,4,6,8,5 (Posición en Referencia a Teclado Numerico de PC) 
                  Activar el 5 pasa a la otra capa. El resto son las flechas.
          Capa 2: Este Modo enciende Leds en la secuencia 7,9,1,3,5 (Posición en Referencia a Teclado Numerico de PC)
                  Funciones: 5 pasa la otra capa
                             1 Click Izq.
                             3 Click Derecho
                             7 Caracter de Escape
                             9 Arrastre
Modo 3: Selección de Pictogramas
        Enciende Columna 1 luego 2 luego 3. En caso de selección enciende led correspondiente a esa columana en las filas 1 luego 2 luego 3.


Conexiones y Pin Out:
Pin	Port map	Características	Función	Descripción
0	  PD3	      Rx, INT3	      USB	
1	  PD2	      Tx, INT2		    USB
2	  PD1	      INT1	          Pulsador user	Pulsador grande selección
3	  PD0	      INT0	          Pulsador modos	Pulsador cambio de modos
4	  PD4		                    LED MODO 1	Indicación modo MOUSE
5	  PC6	      PWM	            LED MODO 2	Indicación modo PICTO
6	  PD7			                  Pulsador Velocidad Luces
7	  PE6	      Rx, INT6	      Bluetooth	
8	  PB4	      Tx	            Bluetooth	
9	  PB5	      PWM	            Buzzer	
10	PB6	      PWM		          Pulsador Velocidad Mouse
16	PB2		    LEDMatrix_Row1	
14	PB3		    LEDMatrix_Row2
15	PB1		    LEDMatrix_Row3	
A0	PF7		    LEDMatrix_Col1	
A1	PF6		    LEDMatrix_Col2
A2	PF5		    LEDMatrix_Col3	
A3	PF4			

