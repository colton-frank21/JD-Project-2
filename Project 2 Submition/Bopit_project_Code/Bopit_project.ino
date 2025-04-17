#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//Final Code For the Bop it Project
// OLED Config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // No reset pin
#define SCREEN_ADDRESS 0x3C // Usually 0x3C or 0x3D depending on your screen


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Variables for your input system
int correctInput = 0;
int micPin = A3;
int threshold = 500;
int micValue = 0;
int score = 0;
int hiscore = 0;
int pins[] = {6,5,A3};
int timer = 3000;
int start = 0;
int reset = 0;

void setup() {
  // Setup buttons & outputs

  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    // If screen fails
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(1); // Bigger text
  display.setTextColor(SSD1306_WHITE);
  randomSeed(analogRead(0)); // Seed for randomness
}



void loop() {
  //Check win condition
   if(score >= 99){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("You Win!!!!!!!!!!!!");
    display.display();
    tone(10, 200);
    delay(100);
    noTone(10);
    delay(100);
    tone(10, 200);
    delay(100);
    noTone(10);
    start = 0;
    delay(500);
    hiscore = score;
    score = 0; 
  }
  //Check Rest Pin
  while(start == 0){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Press Button");
    display.println("To Start Game");
    display.display();
    if(digitalRead(1)==HIGH){
      start = 1;
    }
  }

  correctInput = pins[random(0,3)]; // Pick random pin (5, 6, or 7)

  //Outputting correct tones
  if(correctInput == 5){
    tone(10, 200);
  }
  if(correctInput == 6){
    tone(10, 400);
  }
  if(correctInput == 17){
    tone(10, 600);
  }
  
  //Turning off Tone
  delay(500);
  noTone(10);
  

  if (score > hiscore){
    hiscore = score;
  }


  // Wait for user input
  while (true) {
    //Check Rest Pin
    if (digitalRead(2)==HIGH){
      start = 0;
      score = 0; 
      hiscore = 0;
      break;
    }

    micValue = analogRead(micPin);
    if (digitalRead(5) == HIGH) { 
      checkInput(5); 
      break; 
    }
    if (digitalRead(6) == HIGH) { 
      checkInput(6); 
      break; 
    }
    if (micValue>threshold) { 
      checkInput(A3); 
      break; 
    }

    //Displaying Instructions
    display.clearDisplay();
    display.setCursor(0, 0);

    if(correctInput == 17){
    display.println("TWIST");
    }
    if(correctInput == 5){
    display.println("PUSH");
    }
    if(correctInput == 6){
    display.println("FLIP");
    }
    display.println("Score: "+ String(score));
    display.println("High-Score: "+ String(hiscore));
    display.println(timer);
    display.display();
    //timer check
    if (timer == 0){
      //Force incorrect input
      checkInput(7);
      break;
    }
    
    delay(10);

    if(score <= 5){
      timer -=10;
    }
    else if(score <= 10){
      timer -= 25;
    }
    else if(score <= 15){
      timer -= 50;
    }
    else if(score <= 20){
      timer -= 75;
    }
    else{
      timer -= 100;
    }

  }//end of when true 

  //Add delay between commands
  delay(1000);
}

void checkInput(int userInput) {
  display.clearDisplay();
  if (userInput == correctInput) {
    score += 1;
    display.setCursor(0, 0);
    display.println("Correct!");
    display.display();
    digitalWrite(9, HIGH);
    delay(500);
    digitalWrite(9, LOW);
    timer = 3000;
  } else {
    score = 0;
    display.setCursor(0, 0);
    if(timer > 0){
      display.println("Wrong!");
      start = 0;
      timer = 3000;
      tone(10, 200);
      delay(500);
      noTone(10);
      delay(100);
      tone(10, 200);
      delay(500);
      noTone(10);
      delay(100);
      tone(10, 50);
      delay(1000);
      noTone(10);
    }
    else if (timer <= 0){
      display.println("Out of time!");
      start = 0;
      timer = 3000;
      tone(10, 200);
      delay(500);
      noTone(10);
      delay(100);
      tone(10, 200);
      delay(500);
      noTone(10);
      delay(100);
      tone(10, 50);
      delay(1000);
      noTone(10);
    }
    display.display();
  }
}

