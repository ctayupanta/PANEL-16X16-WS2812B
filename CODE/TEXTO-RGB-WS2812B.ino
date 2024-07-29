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

// Variable global para el texto a mostrar
String displayText = "PCBWAY";

// Variables para controlar la posición del texto
int x = matrix.width();
int y = (matrix.height() - 8) / 2;  // Centramos el texto verticalmente, 8 es la altura de la fuente

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  randomSeed(analogRead(0));  // Semilla para generar números aleatorios
}

void loop() {
  // Desplazar el texto
  matrix.fillScreen(0);

  for (int i = 0; i < displayText.length(); i++) {
    // Generar un nuevo color aleatorio para cada carácter
    uint8_t red = random(0, 256);
    uint8_t green = random(0, 256);
    uint8_t blue = random(0, 256);
    matrix.setTextColor(matrix.Color(red, green, blue));
    
    // Establecer la posición de cada carácter individualmente
    matrix.setCursor(x + (i * 6), y);  // 6 es el ancho de cada carácter
    matrix.print(displayText[i]);
  }

  matrix.show();

  int textWidth = 6 * displayText.length();  // Ancho del texto a mostrar, 6 es el ancho de cada carácter
  if (--x < -textWidth) {
    x = matrix.width();
    delay(500);  // Pausa antes de repetir el texto
  }

  // Verificar si hay nueva entrada en el monitor serial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    
    // Comando para ajustar la posición Y
    if (input.startsWith("Y=")) {
      y = input.substring(2).toInt();
    }
    // Comando para ajustar la posición X
    else if (input.startsWith("X=")) {
      x = input.substring(2).toInt();
    }
    // Comando para actualizar el texto
    else {
      displayText = input;
      x = matrix.width();  // Reiniciar posición del texto
    }
  }

  delay(100);
}
