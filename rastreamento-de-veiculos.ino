#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define PIN_TX    10
#define PIN_RX    11
SoftwareSerial mySerial(PIN_TX, PIN_RX);

boolean LocationIsValid = false;
boolean StopContinuousLocation = false;

TinyGPSPlus gps;
String value;
String phone_number;
String message_text;
String received_message = "loc";
String Message;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.println("Getting single location!");
  phone_number = "+5594984007107";
  getGPS();
  sendLocationSMS();
}

void loop() {

  phone_number = "094981210430";
  if (message_text == received_message) {
    Serial.println("Getting single location!");
    getGPS();
    sendLocationSMS();
  }

}


String SIM808(String value) {
  String Out;

  mySerial.println(value);
  delay(10);
  while (mySerial.available()) {
    Out = (mySerial.readString());
  }

  Out.trim();
  Out.remove(0, value.length() + 3);
  return Out;
}
String SIM808(String value, int count) {
  String Out;

  mySerial.println(value);
  delay(10);
  while (mySerial.available()) {
    Out = (mySerial.readString());
  }

  Out.trim();
  Out.remove(0, value.length() + 3 + count);
  return Out;
}

void getGPS() {
  do {
    Serial.println("Power Down The GPS");
    SIM808("AT+CGPSPWR=0");
    //Serial.println("end");

    Serial.println("Check The GPS Power");
    value = SIM808("AT+CGPSPWR?");
    value.remove(11, 17);
    Serial.println(value);
    //Serial.println("end");

    Serial.println("Power Up The GPS");
    SIM808("AT+CGPSPWR=1");
    //Serial.println("end");

    Serial.println("Disable The GPS Output");
    SIM808("AT+CGPSOUT=0");
    //Serial.println("end\n");

    Serial.println("Wait For The GPS To Find Location");
    while (!SIM808("AT+CGPSSTATUS?" , 13).startsWith("Location 3D Fix")) {
      Serial.println("Location Not Fixed Yet, Please Wait!");
      delay(2000);
    }
    Serial.println("Location Found!");
    Serial.println("end\n");

    //Get and Analyse The GPS Output
    Serial.println("Get and Analyse The GPS Output");
    String Out = "";
    mySerial.println("AT+CGPSOUT=2");

    delay(10);
    while (mySerial.available()) {
      mySerial.readStringUntil('\n');
      mySerial.readStringUntil('\n');
      Out = (mySerial.readStringUntil('\n'));  Out += "\r\n";
    }
    mySerial.println("AT+CGPSOUT=0");
    delay(100);
    mySerial.println("AT+CGPSOUT=32");
    delay(10);
    while (mySerial.available()) {
      mySerial.readStringUntil('\n');
      mySerial.readStringUntil('\n');
      Out += (mySerial.readStringUntil('\n'));  Out += "\r\n";
    }
    mySerial.println("AT+CGPSOUT=0");
    Out.trim();
    Serial.println(Out);
    Serial.println("");
    //GPS Output Analized


    int buffint = Out.length();
    char buff[buffint];
    Out.toCharArray(buff, buffint);
    const char *gpsStream = buff;

    while (*gpsStream)
      if (gps.encode(*gpsStream++))
        displayInfo();

    Serial.println("");
    Serial.println("");

    delay(100);
    if (gps.location.isValid())
    {
      LocationIsValid = true;
      Message = String(gps.location.lat(), 6);
      Message += ",";
      Message += String(gps.location.lng(), 6);
      Message += " ";
      Serial.println(Message);
    }
  } while (!LocationIsValid);
}

void sendLocationSMS() {
  Serial.println("Start Sending The SMS\n");
  Serial.println("Sending The SMS to");
  Serial.println(phone_number);

  SIM808("AT+CMGS=" + phone_number );
  delay(200);
  mySerial.println("Open This Link:");
  mySerial.print("https://www.google.com/maps/place/");
  SIM808(Message);
  Serial.println("Message Sent!");
  Serial.println("Delete All Previous Messages");
  SIM808("AT+CMGD=1,4");
}


void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

/*
 //Starting SMS
  Serial.println("Start\n");
  Serial.println("Check AT Connection");
  value = SIM808("AT");
  Serial.println(value);
  Serial.println("");
  Serial.println("Starting module in SMS Text Mode");
  SIM808("AT+CMGF=1");
  delay(1000);

  Serial.println("Delete All Previous Messages");
  SIM808("AT+CMGD=1,4");
  delay(2000);

  value = SIM808("AT+CMGL=\"REC UNREAD\"");
  value = SIM808("AT+CMGL=\"REC UNREAD\"");
  value = SIM808("AT+CMGL=\"REC UNREAD\"");

  do {
    delay(2000);
    Serial.println("No new Messages Received");
    Serial.println("Check for New Messages!");
    value = SIM808("AT+CMGL=\"REC UNREAD\"");
    //Serial.println("end");
  } while (value == "OK");

  SIM808("AT+CMGL=\"REC UNREAD\"");
  Serial.println("Message Recieved");

  value = SIM808("AT+CMGL=\"ALL\"");
  Serial.println(value);
  phone_number = value;
  phone_number.remove(0, 20);
  phone_number.remove(15, phone_number.length());
  message_text = value;
  message_text.remove(0, 62);
  message_text.remove(message_text.length() - 6, message_text.length());

  Serial.println("Phone Number:");
  Serial.println(phone_number);

  Serial.println("Message Text:");
  Serial.println(message_text);
  Serial.println("//Message end//");
*/
