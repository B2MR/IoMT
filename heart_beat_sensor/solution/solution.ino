//on définit le pin analogique souhaité
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

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  //On définit des valeurs temporaires à nos variables
  //Elles seront modifiées ensuite lors de la prise de mesure
  static float oldValue = 0;

  unsigned long currentMillis = millis();

  //la valeur brute du capteur est récupérée (fréquence en mHz)
  int rawValue = analogRead(sensorPin);
  float value = alpha * oldValue + (1 - alpha) * rawValue;

  change = value-oldValue;

  //Une fois les 15 secondes atteintes, on affiche le BPM
  //Puis on reset les compteurs et on recommence
  if (currentMillis - previousMillis > interval) {
   previousMillis = currentMillis;
   BPM = beatCount * 4;
   Serial.println("********");
   Serial.print("Beat Count: ");
   Serial.println(beatCount);
   Serial.print("BPM: ");
   Serial.println(BPM);
   Serial.println("********");

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
  delay(period);
}
