#include <LoRa.h>
#include "GPS_Main.h"
#include "LoRa_Main.h"
#include <SoftwareSerial.h>

SoftwareSerial gsm(2, 3);

String buffer = "";
String allowedNumber = "+381628775688";  // Dozvoljeni broj
String senderNumber = "";
String messageContent = "";
bool newMessageReceived = false;

TinyGPSPlus gpsClient;
gpsClass gpsObj(gpsClient);

//SoftwareSerial Debug_software(2,3); // Kreiraj SoftwareSerial objekat za komunikaciju sa GPS modulom


char GPSdata[128] = "";

unsigned long current_time=0;
unsigned long onstart_delay=0;


void setup()
{
  Serial.begin(9600);
  //Debug_software.begin(9600);
  gsm.begin(9600);        // GSM komunikacija

  //Nije potrebno ako se koriste standardni - hardverski pinovi
  LoRa.setPins(Atmega.ss, Atmega.rst,Atmega.dio0);

  if (!LoRa.begin(LoRaConfig.FREQUENCY)) 
  {
     // Debug_software.print("LoRa module initialization NOOOOTTT successful!\n\n\n");
  }

 // Debug_software.print("LoRa module initialization is Succes!\n\n\n");

  LoRa.setTxPower(12, PA_OUTPUT_PA_BOOST_PIN);
  onstart_delay=millis();  //PAMTI U KOJOJ SEKUNDI SMO NAPUSTILI setup tacnije kada je krenuo loop da bi posle tamo mogao da ponavljam loop
  setupSIM800L();

}

void loop()
{
//RADIMO WHILE DA BI  TINYGPSPLUSPLUS mogao da encoduje!!
  while (Serial.available()>0)
  {
    if (gpsClient.encode(Serial.read()))
    {
      //Debug_software.println("Procitao je podatke od GPS-a");
      if ((millis()- current_time)>1500 && (millis()-onstart_delay)>3500){
      

        current_time=millis();
        gpsObj.updGPSdata();
        sprintf(GPSdata, "%s,%s\n%s\n%s\n", gpsObj.latBuff, gpsObj.lngBuff, gpsObj.dateBuff, gpsObj.timeBuff);


       /* UKLJUCIVANJE POJACAVACA preko pina 6!!!
        if(HIGH_POWER == LoRaConfig.power)
        {
            digitalWrite(LoRaConfig.HighPowerAmpEna, ON);
            delay(1000);
        }
        */
        LoRa.beginPacket();
        LoRa.print(GPSdata);
        LoRa.endPacket();

        //ISKLJUCIVANJE POJACAVACA preko pina 6!!!
      /*  if(HIGH_POWER == LoRaConfig.power)
        {
          delay(1000);
          digitalWrite(LoRaConfig.HighPowerAmpEna, OFF);
        }
        */
      }
    }
  }

  readSIM800L();

  // Ako je stigla nova poruka
  if (newMessageReceived) {
    if (senderNumber == allowedNumber) {
      sendAutoReply(senderNumber, "45.234228,19.73486");
    } else {
      Serial.println("Poruka nije od dozvoljenog broja, ne odgovaram.");
    }

    // Resetuj
    newMessageReceived = false;
    senderNumber = "";
    messageContent = "";
  }
 
}
//DODATE FUNKCIJE ZA GSM
void setupSIM800L() {
  gsm.println("AT"); //Ovo vraca ok ali to je za test
  delay(1000);
  gsm.println("AT+CMGF=1");            // Postavljamo u rezim tekst moda -AT komande 
  delay(1000);
  gsm.println("AT+CNMI=2,2,0,0,0");    // Automatski prikaz novih SMS-ova
  delay(1000);
}

void readSIM800L() {
  while (gsm.available()) {
    char c = gsm.read();
    buffer += c;

    if (c == '\n') {
      processBufferLine();
      buffer = "";
    }
  }
}

void processBufferLine() {
  buffer.trim();
  if (buffer.length() == 0) return;

  if (buffer.startsWith("+CMT:")) {
    int startQuote = buffer.indexOf('"') + 1;
    int endQuote = buffer.indexOf('"', startQuote);
    senderNumber = buffer.substring(startQuote, endQuote);
  }
  else if (!buffer.startsWith("+CMT:") && !buffer.startsWith("OK")) {
    messageContent = buffer;
    newMessageReceived = true;
  }
}

void sendAutoReply(String phoneNumber, String message) {

  gsm.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(2000);

  gsm.print(message);
  delay(500);

  gsm.write(26); // Ctrl+Z
  delay(5000);

 // Serial.println("Poruka poslata!\n");
}