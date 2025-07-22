#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// --- WiFi Credentials ---
const char* ssid = "NETWORK";
const char* password = "PASSWORD";

// --- OSC Settings ---
WiFiUDP Udp;
const unsigned int localPort = 10000;

// --- Servo Driver Settings ---
#define MIN_PULSE_WIDTH       550
#define MAX_PULSE_WIDTH       2200
#define FREQUENCY             50
#define CENTER_PULSE          1375

// --- I2C Pins (ESP32 Default) ---
#define I2C_SDA 21
#define I2C_SCL 22

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// --- Servo Pin Definitions (Testing one pair) ---
const int yServoPins[] = {0, 2, 4, 6, 8, 10}; // Only servo channel 0 for Y
const int xServoPins[] = {1, 3, 5, 7, 9, 11}; // Only servo channel 1 for X
// The following will automatically be calculated as 1
const int numYServos = sizeof(yServoPins) / sizeof(yServoPins[0]);
const int numXServos = sizeof(xServoPins) / sizeof(xServoPins[0]);


void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA, I2C_SCL);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  Serial.println("PWM Driver Initialized.");

  Serial.println("Centering test servos...");
  // These loops will now only run once because numYServos and numXServos are 1
  for (int i = 0; i < numYServos; i++) {
    moveMotor(yServoPins[i], CENTER_PULSE); // Will call moveMotor(0, CENTER_PULSE)
  }
  for (int i = 0; i < numXServos; i++) {
    moveMotor(xServoPins[i], CENTER_PULSE); // Will call moveMotor(1, CENTER_PULSE)
  }
  Serial.println("Test servos centered.");

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
  Serial.print("MAC Address: ");   
  Serial.println(WiFi.macAddress());


  Serial.print("Starting UDP OSC listener on port ");
  Serial.println(localPort);
  Udp.begin(localPort);
}

void moveMotor(int servoChannel, float pulseWidth) {
  pulseWidth = constrain(pulseWidth, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  int pulseTicks = int(pulseWidth / 1000000.0 * FREQUENCY * 4096);
  pwm.setPWM(servoChannel, 0, pulseTicks);
}

void moveAllYServos(float position) {
  Serial.print("Moving Y Servo (0) to: "); Serial.println(position);
  for (int i = 0; i < numYServos; i++) {
    moveMotor(yServoPins[i], position); 
  }
}

void moveAllXServos(float position) {
  Serial.print("Moving X Servo (1) to: "); Serial.println(position);
  for (int i = 0; i < numXServos; i++) {
    moveMotor(xServoPins[i], position); 
  }
}

void routeYServos(OSCMessage &msg) {
  if (msg.isFloat(0)) {
    float y_position = msg.getFloat(0);
    moveAllYServos(y_position);
  } else {
    Serial.println("OSC Error: /y_servos message did not contain a float.");
  }
}

void routeXServos(OSCMessage &msg) {
  if (msg.isFloat(0)) {
    float x_position = msg.getFloat(0);
    moveAllXServos(x_position);
  } else {
    Serial.println("OSC Error: /x_servos message did not contain a float.");
  }
}

void loop() {
  OSCBundle bundle;
  int packetSize = Udp.parsePacket();

  if (packetSize > 0) {
    while (packetSize--) {
      bundle.fill(Udp.read());
    }
    if (!bundle.hasError()) {
      bundle.dispatch("/y_servos", routeYServos);
      bundle.dispatch("/x_servos", routeXServos);
    } else {
      OSCErrorCode error = bundle.getError();
      Serial.print("OSC Bundle Error: ");
      Serial.println(error);
    }
    bundle.empty();
  }
}