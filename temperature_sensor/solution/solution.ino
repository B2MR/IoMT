/********************************************************************/
// 1. On inclut les librairies utilisées 
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// 2. Le fil pour la data est branché sur le pin 2 de l'Arduino  
#define ONE_WIRE_BUS 2
/********************************************************************/
// 3. Mise en place d'une instance oneWire pour communiquer avec n'importe quel autre device OneWire 
// (pas seulement les Circuits intégrés Maxim/Dallas) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// 4. On passe notre reference oneWire à Dallas Temperature 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 

void setup(void) 
{ 
 // 5. on ouvre le port série et définit le débit de données à 9600 bps
 Serial.begin(9600); 
 // on demarre la libraire 
 sensors.begin(); 
} 

void loop(void) 
{ 
// 6. on appelle sensors.requestTemperatures() pour émettre une température globale
// demande à tous les appareils sur le bus
/********************************************************************/
 sensors.requestTemperatures(); // Envoie la commande pour obtenir des lectures de température
/********************************************************************/
 Serial.print("Temperature : "); 
 Serial.print(sensors.getTempCByIndex(0)); // 7. Pourquoi "byIndex"?
   // Vous pouvez en avoir plusieurs sur le même bus.
   // 0 fait référence au premier IC (Integrated Circuit ou Circuit Integré) sur le fil
   delay(1000); 
}
