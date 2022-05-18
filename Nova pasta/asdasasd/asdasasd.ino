//Programa: Envio de SMS com o modulo SIM808 Arduino
//Autor: Arduino e Cia
//Adaptacao do exemplo da biblioteca Adafruit FONA
#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>
#define SIM808_RX 11
#define SIM808_TX 10
#define SIM808_RST 4
SoftwareSerial SIM808SS = SoftwareSerial(SIM808_TX, SIM808_RX);
SoftwareSerial *SIM808Serial = &SIM808SS;
Adafruit_FONA SIM808 = Adafruit_FONA(SIM808_RST);
char* message = "vai tomar no cu";
char* sendto = "94992597464";
void setup()
{
  Serial.begin(9600);
  Serial.println(F("Teste basico do modulo SIM808"));
  Serial.println(F("Inicializando... (pode demorar alguns segundos)"));
  //Inicializa o SIM808 com velocidade de comunicacao 9600bps
  SIM808Serial->begin(9600);
  //Inicia o modulo
  SIM808.begin(*SIM808Serial);
  Serial.println("Modulo inicializado!");
}
void loop()
{
  //Mostra o IMEI do modulo
  char imei[16] = {0};
  uint8_t imeiLen = SIM808.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Numero IMEI do modulo: ");
    Serial.println(imei);
    Serial.println();
  }
  Serial.println(F("Lendo o RSSI:"));
  uint8_t n = SIM808.getRSSI();
  int8_t r;
  Serial.print(F("RSSI = ")); Serial.print(n); Serial.print(": ");
  if (n == 0) r = -115;
  if (n == 1) r = -111;
  if (n == 31) r = -52;
  if ((n >= 2) && (n <= 30)) {
    r = map(n, 2, 30, -110, -54);
  }
  Serial.print(r); Serial.println(F(" dBm"));
  //Aguarda 3 segundos para enviar o SMS
  delay(3000);
  Serial.println();
  //Enviar SMS
  Serial.print(F("Enviando mensagem SMS"));
  SIM808.sendSMS(sendto, message);
  Serial.println(F("SMS enviado!"));
  //Aguarda 5 minutos e repete o processo
  Serial.println();
  delay(300000);
}
