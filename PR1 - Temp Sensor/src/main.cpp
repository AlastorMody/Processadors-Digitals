#include <Arduino.h>
#include <math.h>
//SENSOR DE TEMPERATURA ANALÓGICO KY-013

int sensorPin = 12; //PIN DEL SENSOR DE TEMP ANALÓGICO
int redLED = 27; //PIN LED ROJO
int yellLED = 26; //PIN LED AMARILLO
int blueLED = 25; //PIN LED AZUL
bool red = false, yell = false, blue = false;
 //Función que transforma la lectura analógica a temperatura
double Thermistor(int RawADC)
{
double Temp;
//Conversión de la lectura RawADC a un valor de Vout
Temp = log (((10240000/RawADC) - 10000));
//Ecuación de Steinhart–Hart que nos convierte el Vout a Temperatura en °K
//los coeficientes en el mismo orden son el A, B, C y se han precalculado para este sensor en concreto
Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp)) * Temp);
//Conversión a Celsius
Temp = Temp - 273.15;
return Temp;
}

void setup()
{
 Serial.begin(115200);
 pinMode(sensorPin, INPUT);
 pinMode(redLED, OUTPUT);
 pinMode(yellLED, OUTPUT);
 pinMode(blueLED, OUTPUT);
 //necesario para qe el rango sea entre 0-1024 y sea coherente con la fórmula
 analogReadResolution(10);
}
void loop()
{    
//Leemos la señal analógica en el PIN definido
 int readVal = analogRead(sensorPin);
 //Serial.println(readVal); //Chivato para ver la resolución del sensor
 //Llamada a la función para obtener la temperatura
 double temp = Thermistor(readVal);
 //Output Temperatura en el Serial
 Serial.print("Current temperature is:"); 
 Serial.print(temp);
 Serial.println("°C");
 Serial.println("-------------------------------------");
 //Condiciones en las que se encienden los LEDs
 if(temp < 21.5 )
 {
    if(yell)
    {
        digitalWrite(yellLED, LOW);
        Serial.println("YELLOW LED OFF");
        yell = false;
    }    
    if(!blue)
    {
        digitalWrite(blueLED, HIGH);
        Serial.println("BLUE LED ON");
        blue = true;
    }
 }
 else if (temp >= 21.5 && temp <= 28.0)
 {
    if(blue)
    {
        digitalWrite(blueLED, LOW);
        Serial.println("BLUE LED OFF");
        blue = false;
    }
    if(red)
    {
        digitalWrite(redLED, LOW);
        Serial.println("RED LED OFF");
        red = false;
    }
    if(!yell)
    {
        digitalWrite(yellLED, HIGH);
        Serial.println("YELLOW LED ON");
        yell = true;
    }
 }
 else
 {     
    if(!red)
    {
        digitalWrite(redLED, HIGH);
        Serial.println("RED LED ON");
        red = true;
    }
    if(yell)
    {
        digitalWrite(yellLED, LOW);
        Serial.println("YELLOW LED OFF");
        yell = false;
    }
 }
 delay(500);
}
