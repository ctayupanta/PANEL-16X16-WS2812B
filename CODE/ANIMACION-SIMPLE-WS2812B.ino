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
  // Animación del muñeco
  for (int i = 0; i < 10; i++) {
    drawStickFigure(0); // Posición inicial
    delay(500);
    drawStickFigure(1); // Posición animada
    delay(500);
  }
}

void drawStickFigure(int frame) {
  // Limpiar la pantalla
  matrix.fillScreen(0);

  // Definir color
  uint16_t white = matrix.Color(255, 255, 255);

  // Dibujar cabeza
  matrix.drawPixel(7, 3, white);
  matrix.drawPixel(8, 3, white);
  matrix.drawPixel(7, 4, white);
  matrix.drawPixel(8, 4, white);

  // Dibujar cuerpo
  matrix.drawLine(8, 5, 8, 10, white);

  // Dibujar brazos
  if (frame == 0) {
    matrix.drawLine(6, 6, 10, 6, white); // Brazos horizontales
  } else {
    matrix.drawLine(6, 5, 8, 6, white); // Brazo izquierdo levantado
    matrix.drawLine(8, 6, 10, 5, white); // Brazo derecho levantado
  }

  // Dibujar piernas
  if (frame == 0) {
    matrix.drawLine(8, 10, 7, 13, white); // Pierna izquierda
    matrix.drawLine(8, 10, 9, 13, white); // Pierna derecha
  } else {
    matrix.drawLine(8, 10, 7, 12, white); // Pierna izquierda levantada
    matrix.drawLine(8, 10, 9, 12, white); // Pierna derecha levantada
  }

  // Mostrar en la matriz
  matrix.show();
}
