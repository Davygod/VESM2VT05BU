 // 2-dimensional array of row pin numbers:
const int row[8] = {
  2, 7, 19, 5, 13, 18, 12, 16
};

// 2-dimensional array of column pin numbers:
const int col[8] = {
  6, 11, 10, 3, 17, 4, 8, 9
};

// 2-dimensional array of pixels:
int pixels[8][8];

// cursor position:
int x = 5;
int y = 5;

// Blinking LED
int sidasta_blikk = false;
int originalPixelValue;

// Time since last INPUT-check
unsigned long last_input_time;

void setup() {
  // initialize the I/O pins as outputs iterate over the pins:
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    // initialize the output pins:
    pinMode(col[thisPin], OUTPUT);
    pinMode(row[thisPin], OUTPUT);
    // take the col pins (i.e. the anode) low to ensure that the LEDS are off:
    digitalWrite(col[thisPin], LOW);
  }

  // initialize the pixel matrix:
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      pixels[x][y] = LOW;
    }
  }
  // PinMode for the Joystick
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  last_input_time = millis();
}

void loop() {
  // read input:
  if (millis() - last_input_time > 500) {
    last_input_time = millis();// Updating last INPUT-time

    // Set the pixel to it's original value
    pixels[x][y] = originalPixelValue;


    readSensors();


    // store the state of the selected pixel
    originalPixelValue = pixels[x][y];

    // Blink
    if (sidasta_blikk == false) {
      Serial.print("CHANGING PIXEL TO HIGH\n");
      pixels[x][y] = HIGH;
      sidasta_blikk = true;
    }
    else if (sidasta_blikk == true) {
      Serial.print("CHANGING PIXEL TO LOW\n");
      pixels[x][y] = LOW;
      sidasta_blikk = false;
    }
  }

  // draw the screen:
  refreshScreen();

}

void readSensors() {

  // Joystick control for X
  if ((analogRead(A0) < 256) && x < 7) {
    x += 1;
  }
  else if ((analogRead(A0) > 768) && x > 0) {
    x -= 1;
  }

  // Joystick control for Y
  if ((analogRead(A1) > 768) && y < 7) {
    y += 1;
  }
  else if ((analogRead(A1) < 256) && y > 0) {
    y -= 1;
  }

  // Draws when push button
  if (digitalRead(0) == false) {
    if (pixels[x][y] == LOW) {
      pixels[x][y] = HIGH;
    }
    else if (pixels[x][y] == HIGH) {
      pixels[x][y] = LOW;
    }
  }


  // set the new pixel position high so that the LED will turn on in the next
  // screen refresh:
  // Þetta munum við nota á eftir
  // pixels[x][y] = HIGH;

  Serial.print(analogRead(A1));
  Serial.print("\n");
  Serial.print(analogRead(A0));
  Serial.print("\n");

}

void refreshScreen() {

  // iterate over the rows (cathodes):
  for (int thisRow = 0; thisRow < 8; thisRow++) {
    // take the row pin (cathode) high:
    digitalWrite(row[thisRow], LOW);
    // iterate over the cols (anode):
    for (int thisCol = 0; thisCol < 8; thisCol++) {
      // get the state of the current pixel;
      int thisPixel = pixels[thisRow][thisCol];
      // when the row is LOW and the col is HIGH,
      // the LED where they meet turns on:
      digitalWrite(col[thisCol], thisPixel);
      // turn the pixel off:
      if (thisPixel == HIGH) {
        digitalWrite(col[thisCol], LOW);
      }
    }
    // take the row pin low to turn off the whole row:
    digitalWrite(row[thisRow], HIGH);
  }
}
