#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// Definir el pin donde está conectado el strip de LEDs y los pulsadores
#define PIN 18
#define BUTTON_UP_PIN 16
#define BUTTON_DOWN_PIN 5
#define BUTTON_LEFT_PIN 12
#define BUTTON_RIGHT_PIN 17

// Crear una matriz de 16x16 LEDs en modo serpiente
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS    + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

// Variables del juego de la serpiente
int snakeLength = 3;
int snakeX[256] = {8, 8, 8}; // Posiciones iniciales de la serpiente
int snakeY[256] = {7, 8, 9};
int foodX, foodY;
int direction = 0; // 0 = arriba, 1 = abajo, 2 = izquierda, 3 = derecha
bool gameOver = false;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.fillScreen(0);
  matrix.show();

  // Configurar los pines de los botones
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

  // Colocar la primera comida
  placeFood();
}

void loop() {
  if (gameOver) {
    displayGameOverScroll();
    resetGame();
  } else {
    moveSnake();
    checkCollisions();
    drawGame();
    delay(200);  // Ajusta este valor para cambiar la velocidad del juego
  }
}

void placeFood() {
  bool validPosition = false;
  while (!validPosition) {
    foodX = random(matrix.width());
    foodY = random(matrix.height());
    validPosition = true;
    for (int i = 0; i < snakeLength; i++) {
      if (snakeX[i] == foodX && snakeY[i] == foodY) {
        validPosition = false;
        break;
      }
    }
  }
}

void moveSnake() {
  // Mover el cuerpo de la serpiente
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  // Mover la cabeza de la serpiente
  if (digitalRead(BUTTON_UP_PIN) == LOW && direction != 1) {
    direction = 0;
  } else if (digitalRead(BUTTON_DOWN_PIN) == LOW && direction != 0) {
    direction = 1;
  } else if (digitalRead(BUTTON_LEFT_PIN) == LOW && direction != 3) {
    direction = 2;
  } else if (digitalRead(BUTTON_RIGHT_PIN) == LOW && direction != 2) {
    direction = 3;
  }

  switch (direction) {
    case 0:
      snakeY[0]--;
      break;
    case 1:
      snakeY[0]++;
      break;
    case 2:
      snakeX[0]--;
      break;
    case 3:
      snakeX[0]++;
      break;
  }
}

void checkCollisions() {
  // Comprobar colisión con la comida
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    placeFood();
  }

  // Comprobar colisión con los bordes
  if (snakeX[0] < 0 || snakeX[0] >= matrix.width() || snakeY[0] < 0 || snakeY[0] >= matrix.height()) {
    gameOver = true;
  }

  // Comprobar colisión con el propio cuerpo
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      gameOver = true;
    }
  }
}

void drawGame() {
  matrix.fillScreen(0);

  // Dibujar la comida
  matrix.drawPixel(foodX, foodY, matrix.Color(255, 0, 0));

  // Dibujar la serpiente
  for (int i = 0; i < snakeLength; i++) {
    matrix.drawPixel(snakeX[i], snakeY[i], matrix.Color(0, 255, 0));
  }

  matrix.show();
}

void displayGameOverScroll() {
  matrix.fillScreen(0);
  matrix.setTextWrap(false);
  matrix.setTextColor(matrix.Color(255, 0, 0));
  matrix.setTextSize(1);

  // Definir el mensaje y la longitud del mensaje
  const char* message = "Game Over";
  int messageLength = 9; // Longitud de "Game Over"

  for (int x = matrix.width(); x >= -messageLength * 6; x--) {
    matrix.fillScreen(0);
    matrix.setCursor(x, 5);
    matrix.print(message);
    matrix.show();
    delay(100);  // Ajusta este valor para cambiar la velocidad del desplazamiento
  }
}

void resetGame() {
  gameOver = false;
  snakeLength = 3;
  snakeX[0] = 8; snakeX[1] = 8; snakeX[2] = 8;
  snakeY[0] = 7; snakeY[1] = 8; snakeY[2] = 9;
  direction = 0;
  placeFood();
}

