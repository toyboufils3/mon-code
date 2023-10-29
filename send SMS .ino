#include <SoftwareSerial.h>  // Inclut la bibliothèque SoftwareSerial pour la communication série

SoftwareSerial sim(3, 2); 

String number = "4622321";  
int ldr = A0;  

int seuilLumiere = 500;  
int etatLumierePrecedente = 0;  
int etatLumiere;  

// Définissez les broches pour la LED RGB
int ledRouge = 6;
int ledVert = 7;
int ledBleu = 8;

void setup() {
   Serial.begin(9600);
   sim.begin(9600);
   delay(1000);
   pinMode(ldr,INPUT);
   pinMode(ledRouge, OUTPUT);
   pinMode(ledVert, OUTPUT);
   pinMode(ledBleu, OUTPUT);
}

void loop() {
   int valeurLumiere = analogRead(ldr);
   Serial.println(valeurLumiere);
   delay(2000);
   etatLumiere = (valeurLumiere < seuilLumiere) ? 0 : 1; 
   if (etatLumiere != etatLumierePrecedente) {
       etatLumierePrecedente = etatLumiere;
       SendMessage();
       Serial.print("Changement d'état de la lumière détecté. Il fait ");   
       Serial.println(etatLumiere == 1 ? "clair." : "sombre.");   
   }
   checkSignalQuality();
}

void SendMessage()
{
   sim.println("AT+CMGF=1");    
   delay(200);
   sim.println("AT+CMGS=\"" + number + "\"\r"); 
   delay(200);
   String etatMessage = (etatLumiere == 1) ? "clair." : "sombre.";   
   String message = "L'état de la lumière a changé. Il fait " + etatMessage; 
   sim.println(message);
   delay(100);
   sim.println((char)26);
   delay(200);
}

void checkSignalQuality() {
   sim.println("AT+CSQ");  
   delay(1000);
   
   String signalQuality = sim.readString();  
   int startIndex = signalQuality.indexOf(": ");
   int endIndex = signalQuality.indexOf(",");
   
   if (startIndex > 0 && endIndex > startIndex) {
       String signalStrength = signalQuality.substring(startIndex + 2, endIndex);
       int strength = signalStrength.toInt();
       
       if (strength >= 21) {  
           digitalWrite(ledVert, HIGH);
           digitalWrite(ledBleu, LOW);
           digitalWrite(ledRouge, LOW);
           Serial.println ("Bonne connexion");
           while(1);
       } else if (strength >= 10) {  
           digitalWrite(ledVert, LOW);
           digitalWrite(ledBleu, HIGH);
           digitalWrite(ledRouge, LOW);
            Serial.println ("faible connexion");
       } else {  
           digitalWrite(ledVert, LOW);
           digitalWrite(ledBleu, LOW);
           digitalWrite(ledRouge, HIGH);
            Serial.println ("pas de connexion");
       }
   }
}
