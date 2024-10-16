/*
 * IRIS V3
 * Smartglasses Project
 * Created by Yifan Jiang
 * 8/18/24
 */

#include <Adafruit_GFX.h>     
#include <Adafruit_ST7789.h>  
#include <SPI.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ButtonEvents.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h> // Graphics
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// APP Variables
bool SCREEN_RETRACTED = false; // Var that represents current screen state
typedef enum { // Selection (Encoder-Based) Setting
  ENCODER_LEFT, // Encoder Twist Left
  ENCODER_RIGHT, // Encoder Twist Right
  ENCODER_NEUTRAL,
} EncoderState;
EncoderState ENCODER_STATE = ENCODER_NEUTRAL;
int APP_LAYER = 0; // Main Screen = 0, Menu = 1, Select1 = 2, Select2 = 3
int SELECTION_IDX = 0;  // Index of view's leftmost item.
int PREV_APP_LAYER = APP_LAYER; 
int PREV_SELECTION_IDX = SELECTION_IDX; 

// Bluetooth Variables
bool BLUETOOTH_CONNECTED = false; 

//Servo Variables
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
uint8_t servonum = 0; // our servo # counter

//Screen Variables
#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Tracks current date/time (MMddhhmm[value of day in week])
String DATE_TIME = "113100004"; 
String OLD_TIME = DATE_TIME;


void setup() {
  Serial.begin(9600); 

  // Init Encoder
  encoderInit(); 

  // Init Bluetooth
  bluetoothInit(); 

  // Init Screen and Clear
  screenStart(); 
  delay(1000); 
  updateWindows(); 
}

void loop() {
  checkEncoderStatus(); // See if we need to update the screen based on the encoder
  bluetoothUpdate(); 
  updateCurrentScene(); 
  // checkUpdates(); 
  delay(5); 
}




// // Check to see if there's anything we need to update. 
// void checkUpdates(){
//   encheckBluetooth();
//   checkTime(); 
//   checkWeather();
// }
