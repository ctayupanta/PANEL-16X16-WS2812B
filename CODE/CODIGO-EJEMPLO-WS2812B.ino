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
  matrix.setBrightness(40); // Puedes ajustar el brillo aquí
  matrix.fillScreen(matrix.Color(255, 0, 0)); // Color blanco para todos los LEDs
  matrix.show();
}

void loop() {
  // No es necesario hacer nada en el loop para mantener todos los LEDs encendidos
}
