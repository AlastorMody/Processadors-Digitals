#include <Arduino.h>

const int led1 = 18;
const int led2 = 19;

void led1on( void * parameter )
{
/* loop forever */
for(;;)
{
digitalWrite(led1, HIGH);
vTaskDelay(2000 / portTICK_PERIOD_MS);
digitalWrite(led1, LOW);
Serial.println("APAGANT: LED1 \n ENCENENT: LED2");
vTaskDelay(2000 / portTICK_PERIOD_MS);

}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}

void led2on( void * parameter )
{
// loop forever 
for(;;)
{
vTaskDelay(2000 / portTICK_PERIOD_MS);
digitalWrite(led2, HIGH);
vTaskDelay(2000 / portTICK_PERIOD_MS);
digitalWrite(led2, LOW);
Serial.println("APAGANT: LED2 \n ENCENENT: LED1");
}
// delete a task when finish,
//this will never happen because this is infinity loop 
vTaskDelete( NULL );
} 

void setup()
{
Serial.begin(112500);
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
/* we create a new task here */
xTaskCreate(
led1on, /* Task function. */
"led1on", /* name of task. */
10000, /* Stack size of task */
NULL, /* parameter of the task */
1, /* priority of the task */
NULL); /* Task handle to keep track of created task */

xTaskCreate(
led2on, // Task function. 
"led2on", // name of task. 
10000, // Stack size of task 
NULL, // parameter of the task 
0, // priority of the task 
NULL); //Task handle to keep track of created task 
}
void loop()
{

}