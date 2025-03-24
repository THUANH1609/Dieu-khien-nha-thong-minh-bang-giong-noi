#include <WiFi.h>
#include <WebServer.h>  
#include <ESP32Servo.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

const char* ssid = "WIFI GIANG VIEN";
const char* password = "dhdn7799";

WebServer server(80);  // Using WebServer instead of ESP32WebServer
Servo myServo;
HardwareSerial mySerial(2); // UART2
DFRobotDFPlayerMini myDFPlayer;

#define SG90 5
#define DEN_PIN_1 32
#define DEN_PIN_2 33
#define DEN_PIN_3 12
#define QUAT_PIN 19

void controlServo(int state) {
  Serial.println(state ? "Opening curtains" : "Closing curtains");
  for (int i = (state ? 0 : 180); (state ? i <= 180 : i >= 0); i += (state ? 20 : -20)) {
    myServo.write(i);
    delay(100);
  }
}

void controlDevice(int pin, const char* name, int state) {
  Serial.printf("%s %s\n", state ? "Opening" : "Closing", name);
  digitalWrite(pin, state ? HIGH : LOW);
}

void controlMusic(int state) {
  if (state) {
    myDFPlayer.play(random(1, 5));
  } else {
    myDFPlayer.stop();
  }
}

void processCommand(String device, int state) {
  if (device == "cua") {
    controlServo(state);
  } else if (device == "den 1") {
    controlDevice(DEN_PIN_1, "light 1", state);
  // } else if (device == "den 2") {
  //   controlDevice(DEN_PIN_2, "light 2", state);
  } else if (device == "den 3") {
    controlDevice(DEN_PIN_3, "light 3", state);
  } else if (device == "nhac") {
    controlMusic(state);
  } else if (device == "quat") {
    controlDevice(QUAT_PIN, "fan", state);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pinMode(DEN_PIN_1, OUTPUT);
  pinMode(DEN_PIN_2, OUTPUT);
  pinMode(DEN_PIN_3, OUTPUT);
  pinMode(QUAT_PIN, OUTPUT);
  digitalWrite(DEN_PIN_1, LOW);
  digitalWrite(DEN_PIN_2, LOW);
  digitalWrite(DEN_PIN_3, LOW);
  digitalWrite(QUAT_PIN, LOW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  myServo.attach(SG90);
  myServo.write(0);

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Khong the ket noi voi DFPlayer Mini:");
    Serial.println("1. Vui long kiem tra lai ket noi!");
    Serial.println("2. Vui long chen the SD!");
    while (true) {
      delay(0); // Vòng lặp vô hạn nếu không kết nối được
    }
  }
  Serial.println("DFPlayer Mini da ket noi thanh cong!");

  myDFPlayer.volume(10); 

  server.on("/control", HTTP_POST, [](){
    if (server.hasArg("device") && server.hasArg("state")) {
      String device = server.arg("device");
      int state = server.arg("state").toInt();
      Serial.println(device);
      Serial.println(state);
      processCommand(device, state);
      server.send(200, "text/plain", "Command received");
    } else {
      server.send(400, "text/plain", "Bad Request - Missing parameters");
    }
  });

  server.begin();
}
void loop() {
  server.handleClient();
}