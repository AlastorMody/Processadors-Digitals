#include <Arduino.h>

const int GLed = 25;
const int Yled = 26;
const int Rled = 27;

void GreenLed( void * parameter )
{
/* loop forever */
for(;;)
{
digitalWrite(GLed, HIGH);
vTaskDelay(5000 / portTICK_PERIOD_MS);
digitalWrite(GLed, LOW);
Serial.println("APAGANT: GreenLed \n ENCENENT: GreenLed");
vTaskDelay(5000 / portTICK_PERIOD_MS);
}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}

void YellowLed( void * parameter )
{
// loop forever 
for(;;)
{
vTaskDelay(4000 / portTICK_PERIOD_MS);
digitalWrite(Yled, HIGH);
vTaskDelay(1000 / portTICK_PERIOD_MS);
digitalWrite(Yled, LOW);
Serial.println("APAGANT: LED2 \n ENCENENT: LED1");
vTaskDelay(5000 / portTICK_PERIOD_MS);
}
// delete a task when finish,
//this will never happen because this is infinity loop 
vTaskDelete( NULL );
} 

void RedLed( void * parameter )
{
// loop forever 
for(;;)
{
vTaskDelay(5000 / portTICK_PERIOD_MS);
digitalWrite(Rled, HIGH);
vTaskDelay(5000 / portTICK_PERIOD_MS);
digitalWrite(Rled, LOW);
Serial.println("APAGANT: LED2 \n ENCENENT: LED1");
}
// delete a task when finish,
//this will never happen because this is infinity loop 
vTaskDelete( NULL );
} 

void setup()
{
Serial.begin(112500);
pinMode(GLed, OUTPUT);
pinMode(Rled, OUTPUT);
pinMode(Yled, OUTPUT);
/* we create a new task here */
xTaskCreate(
GreenLed, /* Task function. */
"GreenLed", /* name of task. */
10000, /* Stack size of task */
NULL, /* parameter of the task */
1, /* priority of the task */
NULL); /* Task handle to keep track of created task */

xTaskCreate(
YellowLed, // Task function. 
"YellowLed", // name of task. 
10000, // Stack size of task 
NULL, // parameter of the task 
1, // priority of the task 
NULL); //Task handle to keep track of created task 

xTaskCreate(
RedLed, // Task function. 
"RedLed", // name of task. 
10000, // Stack size of task 
NULL, // parameter of the task 
1, // priority of the task 
NULL); //Task handle to keep track of created task 

}
void loop()
{

}