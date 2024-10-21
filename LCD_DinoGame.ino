
#include <LiquidCrystal_I2C.h> // I2C-LCD-Bibliothek einbinden

// Erzeuge das LCD-Objekt mit der I2C-Adresse des LCDs (häufig 0x27 oder 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adresse, Anzahl der Spalten und Zeilen


boolean gameOn = false;               // is the game on or are we in the menu
boolean dinoInAir = false;            // is the dino in the air? default is false
int score = 0;                        // player score, amount of trees the player jumped over

int treePositions[] ={16,20,24,28};   // initial positions of the trees before they start moving into the screen

// define the graphical shape of the dino and the tree
// each cell of the 32 cells contains 8 rows of each 5 pixels
// B00111 for example means that this row's first two pixels are dark
// and the last three are bright

  byte dino[8]
  {
    B00000,
    B00111,
    B00101,
    B00111,
    B11100,
    B11110,
    B01100,
    B01100
  };

  byte tree[8] {
    B11011,
    B11011,
    B11011,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100
  };

// --------------------------------------------------------------------------------

int treeMillis = 0;         // amount of milli seconds since the tree has last been moved
int dinoMillis = 0;         // amount of milli seconds since the dino has not touched the ground


// --------------------------------------------------------------------------------
void setup() {
lcd.begin(16,2);
lcd.backlight();    
pinMode(5,INPUT_PULLUP);  // pin 5 of Arduino shall be input for the button


lcd.createChar(2,dino);   // create a custom character for the dino and tree with the pixel code defined above
lcd.createChar(1,tree);   // it is then saved in the LCD. Possible indices for saving are 0-7

lcd.setCursor(0,0);           // setup the start screen
lcd.print("PRESS TO START");


}



// --------------------------------------------------------------------------------

void loop() {
  
static int lastButtonPressed = HIGH;          // securing the state of the last time the loop checked if the button was pressed

int buttonState = digitalRead(5);         

if (buttonState==LOW and gameOn==false){
  
for (int i=0; i<15; i++){
  lcd.setCursor(i,0);
  lcd.print(" ");
  delay(100);
}

  gameOn = true;
}


  
if (gameOn == true){

lcd.setCursor(14,0);
lcd.print("0");

int currentMillis = millis();

if (currentMillis - dinoMillis > 800 and dinoInAir == true){
  lcd.setCursor(3,0);
  lcd.print(" ");
  lcd.setCursor(3,1);
  lcd.write(2);
  dinoInAir = false;
  dinoMillis = currentMillis;
}

if (buttonState == LOW && dinoInAir ==false && lastButtonPressed == HIGH){
  lcd.setCursor(3,1);
  lcd.print(" ");
  lcd.setCursor(3,0);
  lcd.write(2);
  dinoMillis = currentMillis;
  dinoInAir = true;

}

lastButtonPressed = buttonState;

if(currentMillis - treeMillis > 300){


for (int i=0; i<4 ; i++){


int pos = treePositions[i];

if (pos==3 and dinoInAir == false){
 score = 0;
  lcd.setCursor(1,0);
  lcd.print("...GAME OVER");
  gameOn = false;
  treePositions[i] = 16;
  treePositions[i+1] = 20;
  treePositions[i+2] = 24;
  treePositions[i+3] = 28;
  delay(3000);
      for (int i=0; i<16;i++){
        lcd.setCursor(i,1);
        lcd.print(" ");
        lcd.setCursor(i,0);
        lcd.print(" ");
        delay(200);
        }
  lcd.setCursor(0,0);
lcd.print("PRESS TO START");
  return;
}

else if (pos==3 and dinoInAir == true){
  score++;
  lcd.setCursor(14,0);
  lcd.print(" ");
  lcd.setCursor(14,0);
  lcd.print(score);
}




lcd.setCursor(pos,1);
lcd.print(" ");
lcd.setCursor(pos-1,1);
lcd.write(1);




if (pos==0){
  lcd.setCursor(0,1);
  lcd.print(" ");
  lcd.setCursor(15,1);
  lcd.write(1);
  treePositions[i]=15;
}
else{
  treePositions[i]--;
}
  

treeMillis = currentMillis;
}

}
  


}



}
