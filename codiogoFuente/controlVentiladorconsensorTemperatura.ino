#define LM35 0
#define TMP36_LM50 1

//Por si cambiamos el voltage de la placa Arduino de 5.0V a 3.3V
#define ARDUINO_TTLVOLT 5.0
#define ARDUINO_VOLT 3.3
#define ARDUINO_ADC 10

//Constantes globales.
//Mejor hacer un fichero include y usar compilación condicionada en función del sensor utilizado.

//Entrada analógica: sensor de temperatura
const int PIN_SENSOR = A0;
//Definimos un umbral de temperaturas
const float TlimteInferior = 32.0;
const float TlimiteSuperior = 38.0;

//Salida PWM: conectada al ventilador (motor DC)
const int PIN_VENTILADOR = 6;
//int pinVentilador = 6; // the pin where fan is connected
bool estadoVentilador = 0; //inicio parado
int velocidadVentilador = 0;

#include <TMP36.h>
//Objetos y variables globales
TMP36 myTMP36(PIN_SENSOR, ARDUINO_TTLVOLT); //Creamos el objeto para manejo del sensor TMP36

/*!
 *  @brief  Leer temperatura utilizando sensor LM35DZ
 *  @param  pinSensor
 *          Entrada analógica placa Arduino a la que se conecta el sensor. Valor entero (Integer):
 *  @param  v_DC
 *          Alimentación en voltios DC de la placa Arduino. Por defecto 5Voltios. Valor real (Float)
 *  @param  bitsResolucion
 *          Por defecto a 10 bits para compatibilidad con placas AVR. Placas Due, Zero and MKR incorporan un ADC de 12 bits.
 *	@return Valor de temperatura medido en ºC
 */
float getTemperaturaDelLM35(int pinSensor = PIN_SENSOR, float v_DC = 5.0, unsigned int bitsResolucion = 10) {
     float temperatura = NAN;
	 int valorSensor;
	 unsigned int valoresEntradaAnalogica = 2^bitsResolucion;
	 
	 valorSensor = analogRead(pinSensor);
     temperatura = (valorSensor * v_DC * 100.0) / valoresEntradaAnalogica;
	 
     return temperatura;     
  }
    

void setup() {	 
	 pinMode(PIN_VENTILADOR, OUTPUT);
     Serial.begin(9600);
}

void loop() {

     float temperaturaActual = myTMP36.getTempC();
     //float temperaturaActual = getTemperaturaDelLM35();

     Serial.println(temperaturaActual);

     if(estadoVentilador == 0 && temperaturaActual > TlimiteSuperior){
         estadoVentilador = 1;
         digitalWrite(PIN_VENTILADOR, HIGH);   // encender ventilador
		 /*
		 valorPWM = map(temperaturaActual, TlimteInferior, TlimiteSuperior, 32, 255); 
         analogWrite(pinVentilador, velocidadVentilador); //Hacemos girar el ventilador
		 
		 */
    }
     //Comparamos con la temperatura límite inferior del umbral establecido
	 if(estadoVentilador == 1 && temperaturaActual < TlimteInferior){
         estadoVentilador == 0;
         digitalWrite(PIN_VENTILADOR, LOW);   // apagar ventilador
    }
	 delay(1000); // Retardo en esta primera aproximación para una lectura de temperaturas más estable. Mejor si utilizamos smoothing.
}
