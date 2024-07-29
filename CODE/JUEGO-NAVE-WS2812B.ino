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

// Variables del juego de la nave espacial
int playerX = 8; // Posición inicial de la nave
int playerY = 14;
int bulletX = -1;
int bulletY = -1;
bool bulletFired = false;
bool gameOver = false;
int level = 1;
int enemyCount = 1;
unsigned long lastEnemyMoveTime = 0;
const int enemyMoveInterval = 500; // Intervalo de movimiento de enemigos en milisegundos

struct Enemy {
  int x;
  int y;
  bool active;
};

Enemy enemies[10]; // Hasta 10 enemigos

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

  // Colocar los enemigos iniciales
  initializeEnemies();
}

void loop() {
  if (gameOver) {
    displayGameOverScroll();
    resetGame();
  } else {
    movePlayer();
    moveBullet();
    if (millis() - lastEnemyMoveTime > enemyMoveInterval) {
      moveEnemies();
      lastEnemyMoveTime = millis();
    }
    checkCollisions();
    drawGame();
    delay(100);  // Ajusta este valor para cambiar la velocidad del juego
  }
}

void initializeEnemies() {
  for (int i = 0; i < enemyCount; i++) {
    enemies[i].x = random(matrix.width());
    enemies[i].y = random(3); // Empieza en las primeras 3 filas
    enemies[i].active = true;
  }
}

void movePlayer() {
  if (digitalRead(BUTTON_UP_PIN) == LOW && playerY > 0) {
    playerY--;
  } else if (digitalRead(BUTTON_DOWN_PIN) == LOW && playerY < matrix.height() - 1) {
    playerY++;
  } else if (digitalRead(BUTTON_LEFT_PIN) == LOW && playerX > 0) {
    playerX--;
  } else if (digitalRead(BUTTON_RIGHT_PIN) == LOW && playerX < matrix.width() - 1) {
    playerX++;
  }
}

void moveBullet() {
  if (bulletFired) {
    if (bulletY >= 0) {
      bulletY -= 2; // La bala se mueve más rápido
    } else {
      bulletFired = false;
    }
  } else {
    bulletX = playerX;
    bulletY = playerY - 1;
    bulletFired = true;
  }
}

void moveEnemies() {
  static int direction = 1;
  for (int i = 0; i < enemyCount; i++) {
    if (enemies[i].active) {
      enemies[i].x += direction;
      if (enemies[i].x <= 0 || enemies[i].x >= matrix.width() - 1) {
        direction = -direction;
        for (int j = 0; j < enemyCount; j++) {
          enemies[j].y++;
          if (enemies[j].y >= matrix.height()) {
            gameOver = true;
          }
        }
        break;
      }
    }
  }
}

void checkCollisions() {
  // Comprobar colisión de la bala con los enemigos
  for (int i = 0; i < enemyCount; i++) {
    if (bulletFired && enemies[i].active && bulletX == enemies[i].x && bulletY == enemies[i].y) {
      enemies[i].active = false;
      bulletFired = false;
    }
  }

  // Comprobar si todos los enemigos están inactivos
  bool allEnemiesInactive = true;
  for (int i = 0; i < enemyCount; i++) {
    if (enemies[i].active) {
      allEnemiesInactive = false;
      break;
    }
  }

  if (allEnemiesInactive) {
    level++;
    enemyCount = min(level, 10); // Incrementa el número de enemigos hasta 10
    initializeEnemies();
  }

  // Comprobar colisión del jugador con los enemigos
  for (int i = 0; i < enemyCount; i++) {
    if (enemies[i].active && playerX == enemies[i].x && playerY == enemies[i].y) {
      gameOver = true;
    }
  }
}

void drawGame() {
  matrix.fillScreen(0);

  // Dibujar la nave del jugador
  matrix.drawPixel(playerX, playerY, matrix.Color(0, 0, 255));

  // Dibujar los enemigos
  for (int i = 0; i < enemyCount; i++) {
    if (enemies[i].active) {
      matrix.drawPixel(enemies[i].x, enemies[i].y, matrix.Color(255, 0, 0));
    }
  }

  // Dibujar la bala
  if (bulletFired) {
    matrix.drawPixel(bulletX, bulletY, matrix.Color(0, 255, 0));
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
  playerX = 8;
  playerY = 14;
  bulletFired = false;
  level = 1;
  enemyCount = 1;
  initializeEnemies();
}
