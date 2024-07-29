#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// Definir el pin donde está conectado el strip de LEDs
#define PIN 18

// Crear una matriz de 16x16 LEDs en modo serpiente
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS    + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.fillScreen(0);
  matrix.show();
}

void loop() {
  // Mostrar el logo de YouTube con scroll y luego la palabra "suscribete"
  for (int x = 16; x >= -80; x--) {
    if (x >= -16) {
      displayYouTubeLogo(x);
    } else {
      displayText("SUSCRIBETE", x + 30);
    }
    delay(100);
  }
}

void displayYouTubeLogo(int offsetX) {
  // Limpiar la pantalla
  matrix.fillScreen(0);

  // Definir colores
  uint16_t red = matrix.Color(255, 0, 0);
  uint16_t white = matrix.Color(255, 255, 255);

  // Dibujar rectángulo rojo con esquinas redondeadas (YouTube logo background)
  for (int x = 2; x <= 13; x++) {
    for (int y = 3; y <= 12; y++) {
      // Redondear las esquinas
      if ((x == 2 && (y == 3 || y == 12)) || (x == 13 && (y == 3 || y == 12))) {
        continue;
      }
      matrix.drawPixel(x + offsetX, y, red);
    }
  }

  // Dibujar triángulo blanco (YouTube play button)
  int centerX = 6 + offsetX;
  int centerY = 7;
  int triangleHeight = 4;
  int triangleBase = 6;

  for (int y = 0; y < triangleHeight; y++) {
    for (int x = 0; x <= y * triangleBase / triangleHeight; x++) {
      matrix.drawPixel(centerX + x, centerY - y + triangleHeight / 2, white);
      matrix.drawPixel(centerX + x, centerY + y - triangleHeight / 2, white);
    }
  }

  // Mostrar en la matriz
  matrix.show();
}

void displayText(const char* text, int offsetX) {
  // Limpiar la pantalla
  matrix.fillScreen(0);

  // Definir color
  uint16_t red = matrix.Color(255, 0, 0);

  // Configurar color y posición del texto
  matrix.setTextColor(red);
  matrix.setCursor(offsetX, 6); // Ajustar posición vertical si es necesario

  // Mostrar el texto
  matrix.print(text);

  // Mostrar en la matriz
  matrix.show();
}
