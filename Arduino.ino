char c;
#define umbral 1000
#define timeOut 500
#define pinActivacion 7

const char activaURL[12] = "/activa.php";
const char desactivaURL[15] = "/desactiva.php";
const char host[] = "";

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  pinMode(pinActivacion, OUTPUT);
  digitalWrite(pinActivacion, LOW);
  Serial.println("Listo");
}

void loop() {
  limpiaSerialESP();

  //When something is received from the barcode scanner
  if (Serial1.available()) {
    String matricula = "";
    leeMatricula(&matricula);
    bool conexion = conectaAHost();
    bool registroExitoso = enviaRequestRegistro(conexion, &matricula);
    control(registroExitoso, &matricula);
    limpiaSerialLector();
  }
}

//Print everything in the ESP's Serial buffer
void limpiaSerialESP() {
  while (Serial2.available()) {
    c = Serial2.read();
    Serial.print(c);  
  }
}

//Get the matricula (student number) read by the barcode scanner
void leeMatricula(String *matricula) {
  unsigned long timer = millis();
  while (millis() - timer <= timeOut) {
    if (Serial1.available()) {
      c = Serial1.read();
      (*matricula) += c;
      if (c == '\n') break;
      timer = millis();
    }
  }
}

/* Connect the ESP to the host
 * Return "true" if the host connection was successful
 * Return "false" if the host connection failed
 */
bool conectaAHost() {
  Serial2.println("accion"); //Tells the ESP to connect to the host
  String input = "";
  leeRespuestaESP(&input);
  if (input.equals("Host exito\r\n")) return true;
  else return false;
}

//Read the ESP's response
void leeRespuestaESP(String *input) {
  //Wait for a response from the ESP
  while (Serial2.available() == 0) {}
  //Read the response
  c = '\0';
  while (c != '\n') {
    if (Serial2.available()) {
      c = Serial2.read();
      (*input) += c;
    }
  }
  Serial.print(*input);
}

/* Send the POST request to the ESP
 * Returns "true" if the student number was saved successfully
 * Returns "false" if it failed
 */
bool enviaRequestRegistro(bool conexion, String *matricula) {
  if (conexion) {
    //If the connection to the host was successful, send the request
    String request = "matricula=";
    request += (*matricula);
    Serial2.print("POST ");
    Serial2.print(activaURL);
    Serial2.print(" HTTP/1.1\r\nHost: ");
    Serial2.print(host);
    Serial2.print("\r\nAccept: */**\r\nContent-Length: ");
    Serial2.print(request.length() - 2); //removes the \r\n from the character count
    Serial2.print("\r\nContent-Type: application/x-www-form-urlencoded\r\n");
    Serial2.print("Connection: close\r\n\r\n");
    Serial2.print(request);
    Serial2.write(26);

    String input = "";
    leeRespuestaHost(&input);
    if (input.endsWith("Sistema activado.FIN DE DATOS\r\n")) return true;
  }
  return false;
}

//Read the host's response
void leeRespuestaHost(String *input) {
  //Waits for the ESP's response
  while (Serial2.available() == 0) {}
  //Read the response
  while (!input->endsWith("FIN DE DATOS\r\n")) {
    if (Serial2.available()) {
      c = Serial2.read();
      (*input) += c;
    }
  }
  Serial.print(*input);
}

//Keeps the load circuit active as long as the same matricula (student number) is being read
void control(bool registroExitoso, String *matricula) {
  if (registroExitoso) {
    activa(true);
    leeMatriculasNuevas(matricula); 

    //Deactivates the load circuit and registers the departure time
    activa(false);
    bool conexion = conectaAHost();
    enviaRequestDesconexion(conexion);
  }
}

//Activates or deactivates the load circuit
void activa(bool activacion) {
  if (activacion) {
    digitalWrite(pinActivacion, HIGH);
    Serial.println("Activado");
  } else {
    digitalWrite(pinActivacion, LOW);
    Serial.println("Desactivado");
  }
}

/* Continuously read the student's ID card.
 * The load circuit deactivates when the student number stops
 * being read for more than "umbral" seconds, or when a new
 * student number is read.
 */
void leeMatriculasNuevas(String *matricula) {
  String matriculaNueva = "";
  unsigned long timer = millis();
  while (millis() - timer <= umbral) {
    if (Serial1.available()) {
      c = Serial1.read();
      matriculaNueva += c;
      if (c == '\n') {
        if (!matriculaNueva.endsWith(*matricula)) {
          break;
        } else {
          matriculaNueva = "";
        }
      }
      timer = millis();
    }
  }
}

//Send a new HTTP request to register the departure time
void enviaRequestDesconexion(bool conexion) {
  if (conexion) {
    Serial2.print("GET ");
    Serial2.print(desactivaURL);
    Serial2.print(" HTTP/1.1\r\nHost: ");
    Serial2.print(host);
    Serial2.print("\r\nConnection: close\r\n\r\n");
    Serial2.write(26);
    
    String input = "";
    leeRespuestaHost(&input);
  }
}

/* Clean the barcode scanner's Serial buffer so that the next
 * reading is a complete student number
 */
void limpiaSerialLector() {
  c = '\0';
  while (Serial1.available() || c != '\n') {
    c = Serial1.read();
  }
}
