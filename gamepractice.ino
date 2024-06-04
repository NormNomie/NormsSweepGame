#include <Adafruit_CircuitPlayground.h>

// Constant ints
const int numberLevels = 3;
const int levelSpeed[numberLevels] = {200, 100, 50}; //speed for each level
const int maxPoints = 5;

// Variables
int level = 0;
int currentLight = 0;
int score = 0;
bool gamePlaying = true;

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  welcomeMessage();
}

void loop() {
  if (CircuitPlayground.slideSwitch()) {
    resetGame();
  } else if (gamePlaying) {
    playLevel(level);
    delay(1000); // 1-second delay between levels
    if (level < numberLevels - 1) {
      level++;
    } else {
      gamePlaying = false;
      printTotalScore();
      endingSong();
      CircuitPlayground.clearPixels();
    }
  }
}

void playLevel(int level) {
  bool levelRunning = true;
  int pointsAwarded = 0;
  while (levelRunning) {
    for (currentLight = 0; currentLight < 10; currentLight++) {
      if (CircuitPlayground.leftButton() || CircuitPlayground.rightButton()) {
        int points = pointsWon(currentLight);
        score += points;
        displayScore(points);
        levelRunning = false;
        break;
      }
      mainSweep(currentLight);
      delay(levelSpeed[level]);
    }
  }
  displayScore(pointsAwarded);
}


void mainSweep(int light) {
  CircuitPlayground.clearPixels();
  for (int i = 0; i < 10; i++) {
    if (i == light) {
      switch (pointsWon(i)) {
        case 5: CircuitPlayground.setPixelColor(i, 0, 0, 255); break; // Blue for 5 points
        case 4: CircuitPlayground.setPixelColor(i, 128, 0, 128); break; // Purple for 4 points
        case 3: CircuitPlayground.setPixelColor(i, 255, 165, 0); break; // Orange for 3 points
        case 2: CircuitPlayground.setPixelColor(i, 255, 255, 0); break; // Yellow for 2 points
        case 1: CircuitPlayground.setPixelColor(i, 0, 255, 0); break; // Green for 1 point
        default: CircuitPlayground.setPixelColor(i, 255, 0, 0); break; // Red for all others
      }
    } else {
      CircuitPlayground.setPixelColor(i, 255, 0, 0); // Red light for the rest
    }
  }
}

int pointsWon(int light) {
  int points = 0;
  switch (light) {
    case 4: points = maxPoints; break;
    case 3: case 5: points = 4; break;
    case 2: case 6: points = 3; break;
    case 1: case 7: points = 2; break;
    case 0: case 8: points = 1; break;
    default: points = 0; break;
  }
  return points;
}

void displayScore(int points) {
  CircuitPlayground.clearPixels();
  for (int i = 0; i < points; i++) {
  CircuitPlayground.setPixelColor(i, 255, 255, 255); // White light for points awarded
  CircuitPlayground.playTone(500, 200); 
  }
  delay(1000); // Display points for 1 second
  CircuitPlayground.clearPixels();
}

void printTotalScore() {
  Serial.print("FINAL SCORE: ");
  Serial.println(score);
  Serial.println("Remember to Reset the Game!");
}


void endingSong() {
  CircuitPlayground.clearPixels();
  for (int i = 0; i < 10; i++) {
  CircuitPlayground.setPixelColor(i, 72, 43, 47); // Rose Gold light for ending
  }
  CircuitPlayground.playTone(800, 100); delay(200);
  CircuitPlayground.playTone(200, 100); delay(200);
  CircuitPlayground.playTone(500, 100); delay(200);
  CircuitPlayground.playTone(200, 100); delay(200);
  CircuitPlayground.playTone(800, 100); delay(200);
}

void welcomeMessage() {
  Serial.println("Welcome to Sweep Challenge!");
  Serial.println("Flip the switch to start the game.");
  delay(2000);
  CircuitPlayground.clearPixels();
}

void resetGame() {
  level = 0;
  score = 0;
  gamePlaying = true;
  delay(1000);
  welcomeMessage();
}
