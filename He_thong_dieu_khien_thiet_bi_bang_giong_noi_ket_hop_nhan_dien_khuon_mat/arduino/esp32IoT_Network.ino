#include <WiFi.h>
#include <WebServer.h>  
#include <ESP32Servo.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

// Cấu hình thông tin WiFi
const char* ssid = "IoT Network";
const char* password = "aiotlab@123";

// Cấu hình Static IP
IPAddress local_ip(192, 168, 162, 5);  // Địa chỉ IP tĩnh
IPAddress gateway(192, 168, 160, 1);   // Địa chỉ Gateway
IPAddress subnet(255, 255, 248, 0);    // Subnet mask

WebServer server(80);  
Servo myServo;
HardwareSerial mySerial(2); 
DFRobotDFPlayerMini myDFPlayer;

// Chân kết nối thiết bị
#define SG90 5
#define DEN_PIN_1 32
//#define DEN_PIN_2 33
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
  Serial.printf("%s %s\n", state ? "Turning ON" : "Turning OFF", name);
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
//   } else if (device == "den 2") {
//     controlDevice(DEN_PIN_2, "light 2", state);
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

  // Cấu hình địa chỉ IP tĩnh
  WiFi.config(local_ip, gateway, subnet);
  WiFi.begin(ssid, password);

  // Cấu hình chân đầu ra
  pinMode(DEN_PIN_1, OUTPUT);
  //pinMode(DEN_PIN_2, OUTPUT);
  pinMode(DEN_PIN_3, OUTPUT);
  pinMode(QUAT_PIN, OUTPUT);
  digitalWrite(DEN_PIN_1, LOW);
  //digitalWrite(DEN_PIN_2, LOW);
  digitalWrite(DEN_PIN_3, LOW);
  digitalWrite(QUAT_PIN, LOW);

  // Chờ kết nối WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Kiểm tra IP

  // Khởi tạo UART2 cho DFPlayer Mini
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  myServo.attach(SG90);
  myServo.write(0);

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Không thể kết nối với DFPlayer Mini:");
    Serial.println("1. Vui lòng kiểm tra lại kết nối!");
    Serial.println("2. Vui lòng chèn thẻ SD!");
    while (true) {
      delay(0);
    }
  }
  Serial.println("DFPlayer Mini đã kết nối thành công!");
  myDFPlayer.volume(20);

  // Định nghĩa API điều khiển thiết bị
  server.on("/control", HTTP_POST, []() {
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