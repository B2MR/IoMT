// Voici les librairies à utiliser
/********************************************************************/
                     //ACCELEROMETRE
/********************************************************************/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/********************************************************************/
                      //TEMPERATURE
/********************************************************************/
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2


// On attribue un identifiant unique à l'accéléromètre
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
// La declaration et initialisation des variables x, y et z de l'accelerometre
float xa = 0;
float ya = 0;
float za = 0;

float at = 0;


// Mise en place d'une instance oneWire pour communiquer avec n'importe quel autre device OneWire
// (pas seulement les Circuits intégrés Maxim/Dallas Temperature)
OneWire oneWire(ONE_WIRE_BUS);
// On passe notre reference oneWire à Dallas Temperature.
DallasTemperature sensors(&oneWire);


/********************************************************************/
                        //CARDIAQUE
/********************************************************************/
//On définit le pin analogique souhaité
int sensorPin = 0;

//L'alpha fait partie du calcul de la valeur de fréquence du capteur
//Il doit être égal à 0.75
float alpha = 0.75;
int period = 50;

//change va correspondre à la différence de lumière
//perçu par le phototransistor entre chaque période.
float change = 0.0;

// Pic max
float maxx = 0.0;

//L'interval de calcul sera sur 15 secondes.
//On calcule le nombre de battements durant cette période
//Puis on multiplie beatCount par 4 afin d'obtenir le BPM
unsigned long previousMillis = 0;
unsigned long interval = 15000;
int beatCount = 0;
int BPM = 0;


/********************************************************************/

int counterTemperature = 0;
int counterAccelerometer = 0;

/********************************************************************/


void setup()
{
  Serial.begin(9600);
  // Initialisation la libraire de temperature
  sensors.begin();
  // Initialisation de l'accelerometre
  if(!accel.begin())
  {
    // Affiché lorsqu'il y a eu un probleme pour detecter le capteur ADXL345.
    //Verifiez vos connections
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
}

void loop()
{

/********************************************************************/
                    //ACCELEROMETRE
/********************************************************************/
    if (counterAccelerometer == 8){

      //Let's code !

      counterAccelerometer = 0;
      //delay(400);   // On remplace le delay de 400 par counterAccelerometer
    }


/********************************************************************/
                    //TEMPERATURE
/********************************************************************/

  if (counterTemperature == 20){

    //Let's code !

    counterTemperature = 0;
    //delay(1000);           // On remplace le delay de 1000 par counterTemperature
    }

/********************************************************************/
                    //CARDIAQUE
/********************************************************************/

  //Let's code !

  delay(period);
}




/********************************************************************/
                    //FONCTIONS SUPPLEMENTAIRES
/********************************************************************/

//Methode de détection de chute
boolean fallDetection(float totalAcceleration)
{
  //si l'accélération totale dépasse la valeur arbitraire de 7,
  //La personne à chutée, renvoie True

}

/********************************************************************/
// Methode à utiliser dans le loop afin de voir si la personne
// possède une temperature trop élevée
boolean highTemperature(float temperature)
{
  //Si la température de la personne dépasse 38,
  //Renvoie true
}
