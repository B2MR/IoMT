/********************************************************************/
// 1. On inclut les librairies utilisées 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
/********************************************************************/
// 2. On attribue un identifiant unique à ce capteur
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
/********************************************************************/
// 3. on déclare et initialise les variables x, y et z de l'acceleromètre
float xa = 0;
float ya = 0;
float za = 0;
/********************************************************************/

void setup(void) 
{ 
   // 4. on ouvre le port série et définit le débit de données à 9600 bps
  Serial.begin(9600);
  Serial.println("ADXL345 Accelerometer Calibration"); 
  Serial.println("");
  
   // 5. Initialisation du capteur
  if(!accel.begin())
  {
    // Affiché lorsqu'il y a eu un probleme pour detecter le capteur ADXL345. Verifiez vos connexions */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
}

void loop(void)
{
    
    // 6. Obtenir un nouvel evenement du capteur
    sensors_event_t accelEvent;  
    accel.getEvent(&accelEvent);

    // 7. Valeurs x, y et z captées par l'evenement de l'acceleromètre
    float xa = accelEvent.acceleration.x;
    float ya = accelEvent.acceleration.y;
    float za = accelEvent.acceleration.z;
    
    //8. Calcul pour obtenir l'acceleration totale
    float at = sqrt(xa*xa + ya*ya + za*za);          
   
    Serial.print("X Axis: "); 
    Serial.println(xa);
    Serial.print("Y Axis: "); 
    Serial.println(ya);
    Serial.print("Z Axis: "); 
    Serial.println(za);
    Serial.print("Total Acceleration: "); 
    Serial.println(at);
}
