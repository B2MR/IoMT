//On inclut les librairies utilisées
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
/********************************************************************/
                             //MQTT
/********************************************************************/
#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
#include <ESP8266WiFi.h>
#endif

#include "arduino_secrets.h"


// on attribue un identifiant unique à ce capteur
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
//La declaration et initialisation des variables x, y et z de l'acceleromètre
float xa = 0;
float ya = 0;
float za = 0;


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

float accelerometre[3] = {0,0,0};
int pulse[1] = {0};
float temp[1] = {0};


// On insere les données sensibles dans arduino_secrets.h
char ssid[] = SECRET_SSID;    // SSID reseau wifi
char pass[] = SECRET_PASS;    // mot de passe wifi

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "XXX.XXX.X.XX"; // remplacez par votre addresse ip
int port = 1883;

const char topicACC[]  = "IoMT/sensor/acc";
const char topicTEMP[]  = "IoMT/sensor/temp";
const char topicPULSE[]  = "IoMT/sensor/pulse";
const char topicFEVER[]  = "IoMT/sensor/fever";
const char topicFALL[]  = "IoMT/sensor/fall";

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

  /********************************************************************/
                             //SETUP MQTT
  /********************************************************************/
  // Tentative de connexion au réseau Wifi:
  Serial.print("Attempting to connect to Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(1000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

/********************************************************************/

}


void loop()
{
  // On appelle la methode poll() pour éviter d'être déconnecté par le broker
  mqttClient.poll();


/********************************************************************/
                    //ACCELEROMETRE
/********************************************************************/
    if (counterAccelerometer == 8){
      // Obtenir un nouvel evenement du capteur
      sensors_event_t accelEvent;
      accel.getEvent(&accelEvent);

      // Valeurs x, y et z captées par l'evenement de l'acceleromètre
      float xa = accelEvent.acceleration.x;
      float ya = accelEvent.acceleration.y;
      float za = accelEvent.acceleration.z;

      accelerometre[0] = xa;
      accelerometre[1] = ya;
      accelerometre[2] = za;

      // Calcul pour obtenir l'acceleration totale
      float at = sqrt(xa*xa + ya*ya + za*za);

      Serial.println("********");
      Serial.print("X Axis: ");
      Serial.println(xa);
      Serial.print("Y Axis: ");
      Serial.println(ya);
      Serial.print("Z Axis: ");
      Serial.println(za);
      Serial.print("Total Acceleration: ");
      Serial.println(at);

      /* Envoi MQTT des données de chute */
      //fallDetection(at);
      if (fallDetection(at)){
        mqttClient.beginMessage(topicFALL);
        mqttClient.print(true);
        mqttClient.endMessage();
      }

      Serial.println("********");

      counterAccelerometer = 0;
      //delay(400);   // On remplace le delay de 400 par counterAccelerometer

      /* Envoi MQTT des données x, y et z de l'accelerometre */
      mqttClient.beginMessage(topicACC);
      mqttClient.print("{\"X\":"+ String(accelerometre[0]) +",\"Y\":"+ String(accelerometre[1]) +",\"Z\":"+ String(accelerometre[2]) +"}");
      mqttClient.endMessage();
    }


/********************************************************************/
                    //TEMPERATURE
/********************************************************************/

  if (counterTemperature == 20){
    // on appelle sensors.requestTemperatures() pour émettre une température globale
    // cela interroge tous les appareils sur le bus
    sensors.requestTemperatures();

     Serial.println("********");
     Serial.print("Temperature : ");
     float temperature = sensors.getTempCByIndex(0);
     // Pourquoi "byIndex"?
     // Vous pouvez en avoir plusieurs sur le même bus.
     // 0 fait référence au premier IC (Integrated Circuit ou Circuit Integré) sur le fil

     temp[0] = temperature;
     Serial.println(temperature);

     //highTemperature(temperature);

      /* Envoi MQTT des données de fievre */
      if (highTemperature(temperature)){
        mqttClient.beginMessage(topicFEVER);
        mqttClient.print(true);
        mqttClient.endMessage();
      }

     Serial.println("********");

     counterTemperature = 0;
     //delay(1000);           // On remplace le delay de 1000 par counterTemperature

     /* Envoi MQTT des données de temperature */
     mqttClient.beginMessage(topicTEMP);
     mqttClient.print(temp[0]);
     mqttClient.endMessage();
  }

/********************************************************************/
                    //CARDIAQUE
/********************************************************************/

  //On définit des valeurs temporaires à nos variables
  //Elles seront modifiées ensuite lors de la prise de mesure
  static float oldValue = 0;
  static float oldChange = 0;

  unsigned long currentMillis = millis();

  //la valeur brute du capteur est récupérée (fréquence en mHz)
  int rawValue = analogRead(sensorPin);
  float value = alpha * oldValue + (1 - alpha) * rawValue;

  change = value - oldValue;

  //Une fois les 15 secondes atteintes, on affiche le BPM
  //Puis on reset les compteurs et on recommence
  if (currentMillis - previousMillis > interval) {
   previousMillis = currentMillis;
   BPM = beatCount * 4;
   pulse[0] = BPM;
   Serial.println("********");
   Serial.print("Beat Count: ");
   Serial.println(beatCount);
   Serial.print("BPM: ");
   Serial.println(BPM);
   Serial.println("********");

   /* Envoi MQTT des données du rythme cardiaque */
   mqttClient.beginMessage(topicPULSE);
   mqttClient.print(pulse[0]);
   mqttClient.endMessage();

   beatCount = 0;
  }

  //Pour chaque nouveau pic, on reset le max
  if (change >= maxx) {
    maxx= change;
    Serial.println("  |");
    beatCount = beatCount + 1;
  } else {
    Serial.println("|");
  }
  //Décroissance lente de max lorsque le capteur est en mesure,
  //mais la décroissance doit être plus lente que le temps nécessaire
  //pour atteindre le prochain pic de battement cardiaque.
  maxx = maxx * 0.98;

  oldValue=value;
  oldChange=change;

  counterTemperature++;
  counterAccelerometer++;

  delay(period);
}


/********************************************************************/
                    //FONCTIONS SUPPLEMENTAIRES
/********************************************************************/

//Methode de détection de chute
boolean fallDetection(float totalAcceleration)
{
  //Chiffre '7' arbitraire a changer au moment de faire les tests
  //de chute libre en regardant le resultat de l'acceleration total
  if (totalAcceleration < 7) {
       Serial.println("Fall Detected");
       return true;
   }
   return false;
}

/********************************************************************/
// Methode à utiliser dans le loop afin de voir si la personne
// possède une temperature trop élevée
boolean highTemperature(float temperature)
{
  if (temperature >= 38.0)
  {
    Serial.println("Fever detected");
    return true;
  }
  return false;
}
