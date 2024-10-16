//Encoder
#define encoderPinA 2
#define encoderPinB 3
#define encoderBtnPin 4

// Encoder Variables
int debounceTime = 0;
int debouncePeriod = 125;  // 125 Milliseconds.

// Button Variables
// https://github.com/fasteddy516/ButtonEvents/blob/master/examples/Basic/Basic.ino 
ButtonEvents encoderBtn; // Button Events Object
int btnDebouncePeriod = 25;  // 50 millis
int btnDoubleTapPeriod = 250; 
int btnHoldPeriod = 500; 

void encoderInit(){
  // Rotary Encoder
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(encoderBtnPin, INPUT_PULLUP);

  encoderBtn.attach(encoderBtnPin); // ButtonEvents
  encoderBtn.debounceTime(btnDebouncePeriod);
  encoderBtn.doubleTapTime(btnDoubleTapPeriod); 
  encoderBtn.holdTime(btnHoldPeriod); 

  // Link the readEncoder function to be called whenever encoderPinA changes
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderRespond, CHANGE);
}

// Update the encoder state based on the direction that we're turning the encoder
void encoderRespond(){
  if (millis() - debounceTime > debouncePeriod) {
    debounceTime = millis();  // Reset our timer.

    if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
      ENCODER_STATE = ENCODER_LEFT;
    } else {
      ENCODER_STATE = ENCODER_RIGHT;
    }
  }
}

void checkEncoderStatus(){
  if(encoderBtn.update() || (ENCODER_STATE != ENCODER_NEUTRAL)) { // If state change occurs
    switch(encoderBtn.event()){
      case(hold):
        SCREEN_RETRACTED = !SCREEN_RETRACTED; // Toggle Screen_Retracted
        // toggleServo(); 
        toggleScreen(); 
        Serial.println("HOLD EVENT");
        break;
      case(doubleTap): // Go back
        if(!SCREEN_RETRACTED && canMakeMove(-1)){
          APP_LAYER--; //Move down a layer (pressed BACK)
          Serial.println("OUT LAYER"); 
          updateWindows(); 
        }
        break;
      case(tap):
        if(!SCREEN_RETRACTED && canMakeMove(1)){
          APP_LAYER++; //Move up a layer (pressed SELECT)
          Serial.println("IN LAYER");
          updateWindows();
        }
        break; 

      default: // Update based on encoder positions. 
        if(ENCODER_STATE != ENCODER_NEUTRAL){
          if(APP_LAYER == 1){
            switch(ENCODER_STATE){
              case(ENCODER_RIGHT):
                if(canMakeMove(2)){
                  SELECTION_IDX++;
                  Serial.println("Right MOVE");
                  Serial.println(SELECTION_IDX); 
                  updateWindows();
                }
                break;
              case(ENCODER_LEFT):
                if(canMakeMove(3)){
                  SELECTION_IDX--;
                  Serial.println("Left MOVE");
                  Serial.println(SELECTION_IDX); 
                  updateWindows(); 
                }
                break;
            }
          }
        }
        ENCODER_STATE = ENCODER_NEUTRAL; // Reset ENCODER STATE to Neutral
        break;
    }
  }
}

bool canMakeMove(int selection){
  if(selection == -1){ // BACK REQ
    return(APP_LAYER != 0); // No if the current layer is 0, otherwise yes.
  }
  if (selection == 1){ // Select REQ
    // Yes if layer is less than 3 and if a second window to select to exists. 
    return((APP_LAYER != 3)  || ((APP_LAYER == 2) && selectionIsNull(SELECTION_IDX)));
  }
  if((selection == 2) || (selection == 3)){ // Left/Right rotation
    return(APP_LAYER == 1); // Yes as long as we are on the MENU layer. 
  }
}
