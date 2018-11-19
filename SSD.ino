#include <GSM.h>
#include <GSM3CircularBuffer.h>
#include <GSM3IO.h>
#include <GSM3MobileAccessProvider.h>
#include <GSM3MobileCellManagement.h>
#include <GSM3MobileClientProvider.h>
#include <GSM3MobileClientService.h>
#include <GSM3MobileDataNetworkProvider.h>
#include <GSM3MobileMockupProvider.h>
#include <GSM3MobileNetworkProvider.h>
#include <GSM3MobileNetworkRegistry.h>
#include <GSM3MobileServerProvider.h>
#include <GSM3MobileServerService.h>
#include <GSM3MobileSMSProvider.h>
#include <GSM3MobileVoiceProvider.h>
#include <GSM3ShieldV1.h>
#include <GSM3ShieldV1AccessProvider.h>
#include <GSM3ShieldV1BandManagement.h>
#include <GSM3ShieldV1BaseProvider.h>
#include <GSM3ShieldV1CellManagement.h>
#include <GSM3ShieldV1ClientProvider.h>
#include <GSM3ShieldV1DataNetworkProvider.h>
#include <GSM3ShieldV1DirectModemProvider.h>
#include <GSM3ShieldV1ModemCore.h>
#include <GSM3ShieldV1ModemVerification.h>
#include <GSM3ShieldV1MultiClientProvider.h>
#include <GSM3ShieldV1MultiServerProvider.h>
#include <GSM3ShieldV1PinManagement.h>
#include <GSM3ShieldV1ScanNetworks.h>
#include <GSM3ShieldV1ServerProvider.h>
#include <GSM3ShieldV1SMSProvider.h>
#include <GSM3ShieldV1VoiceProvider.h>
#include <GSM3SMSService.h>
#include <GSM3SoftSerial.h>
#include <GSM3VoiceCallService.h>

int soundDetectedVal;
int lastSoundDetectTime;
int bAlarm;
int soundAlarmTime; 

void setup ()
{
  Serial.begin(9600);  
  pinMode (7, INPUT) ; // input from the Sound Detection Module
}
void loop ()
{
  soundDetectedVal = digitalRead (13) ; // read the sound alarm time
  
  if (soundDetectedVal == LOW) // If we hear a sound
  {
  
    lastSoundDetectTime = millis(); // record the time of the sound alarm
    // The following is so you don't scroll on the output screen
    if (!bAlarm){
      Serial.println("LOUD, LOUD");
      bAlarm = true;
    }
  }
  else
  {
    if( (millis()-lastSoundDetectTime) > soundAlarmTime  &&  bAlarm){
      Serial.println("quiet");
      bAlarm = false;
    }
  }




   // initialize serial communications and wait for port to open:
   Serial.begin(9600);
   while (!Serial) {
     ; // wait for serial port to connect. Needed for native USB port only
   }

   Serial.println("SMS Messages Sender");

   // connection state
   boolean notConnected = true;

   // Start GSM shield
   // If your SIM has PIN, pass it as a parameter of begin() in quotes
   while (notConnected) {
     if (Serial.begin(10) == GSM_READY) {
       notConnected = false;
     } else {
       Serial.println("Not connected");
       delay(1000);
     }
   }

   Serial.println("GSM initialized");
}

void loop() {

   Serial.print("Enter a mobile number: 8376052927 ");
   char remoteNum[20];  // telephone number to send sms
   readSerial(remoteNum);
   Serial.println(remoteNum);

   // sms text
   Serial.print("Now, enter SMS content: sound sensed ");
   char txtMsg[200];
   readSerial(txtMsg);
   Serial.println("SENDING");
   Serial.println();
   Serial.println("Message:sound sensed");
   Serial.println(txtMsg);

   // send the message
   sms.beginSMS(remoteNum);
   sms.print(txtMsg);
   sms.endSMS();
   Serial.println("\nCOMPLETE!\n");
}

/*
   Read input serial
  */
int readSerial(char result[]) {
   int i = 0;
   while (1) {
     while (Serial.available() > 0) {
       char inChar = Serial.read();
       if (inChar == '\n') {
         result[i] = '\0';
         Serial.flush();
         return 0;
       }
       if (inChar != '\r') {
         result[i] = inChar;
         i++;
       }
     }
   }
}
