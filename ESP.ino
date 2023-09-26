#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define limite 1000 //Time limit to wait for Serial input
#define respuestaTimeout 7000 //Time limit to wait for a response from Apache

const char ssid[] = "";
const char ssidPass[] = "";
const char host[] = "";

char c;
String input;

unsigned long timer;

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.print(F("Conectando a "));
  Serial.print(ssid);
  //Connect to the internet
  WiFi.begin(ssid, ssidPass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(F("Conectado"));
  input = "";
}

void loop() {
  if (Serial.available()) {
    mensajeDeArduino();
    acciones();
  }
}

//Read and store a message from the Arduino
void mensajeDeArduino() {
  input = "";
  timer = millis();
  while (millis() - timer <= limite && !input.endsWith(F("\r\n"))) {
    yield();
    if (Serial.available()) {
      timer = millis();
      c = Serial.read();
      input += c;
    }
  }
}

//Perform an action indicated by the Arduino's message
void acciones() {
  if (input.equals(F("AT\r\n"))) {
    //"AT\r\n" is simply to test the connection between the ESP and the Arduino
    Serial.println(F("OK"));
  } else if (input.equals(F("accion\r\n"))) {
    //"accion\r\n" means to send an HTTP request to the host
    WiFiClient cliente;
    if (!cliente.connect(host, 80)) {
      Serial.println(F("Host fallo"));
    } else {
      Serial.println(F("Host exito"));
      enviaDatosAlHost(&cliente);
      respuestaDeHost(&cliente);
      Serial.println("FIN DE DATOS");
      cliente.stop();
    }
  }
}

//Receive data from the Arduino y send it to the host
void enviaDatosAlHost(WiFiClient *cliente) {
  timer = millis();
  c = '\0';
  //26 is used as a special character to end the data stream
  while (c != 26 && millis() - timer <= limite) {
    yield();
    if (Serial.available()) {
      c = Serial.read();
      if (c != 26) cliente->print(c);
      timer = millis();
    }
  }
}

//Get the host's response
void respuestaDeHost(WiFiClient *cliente) {
  //Wait for the host's response
  Serial.println("Esperando respuesta...");
  timer = millis();
  while (cliente->available() == 0
          && millis() - timer <= respuestaTimeout) {
    yield();
  }
  //Prints the host's response, if there was one
  if (cliente->available()) {
    timer = millis();
    while (millis() - timer <= limite) {
      yield();
      if (cliente->available()) {
        timer = millis();
        c = cliente->read();
        Serial.print(c);
      }
    }
  }
}
