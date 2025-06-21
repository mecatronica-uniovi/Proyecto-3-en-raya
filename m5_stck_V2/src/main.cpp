#include <M5Stack.h> 
#include <M5ez.h>   
#include <ezTime.h> 
#include <WiFi.h> 
#include <Wire.h> 
#include "images.h"
#include <WiFiClient.h>
#include <MyServer.h>

#undef min

//Se definen las variables para poder usar el altavoz
#define BUZZER_PIN 25
#define BUZZER_CHANNEL 0

// Variables globales
float Num1 = 0;
float Num2 = 0;
char Fun;
int CalNum = 0;
int SumFlag = 0;
float Sum;
String Datain;
uint8_t key_val;
int dificultad = 0;

String coor_x = "";
String coor_y = "";
String coor_z = "";

//Declaracion de variables para dibujar el tablero
const uint32_t COLOR_VACIO = TFT_LIGHTGREY; 
const uint32_t COLOR_OCUPADO_ROBOT = TFT_RED;   
const uint32_t COLOR_OCUPADO_JUGADOR = TFT_GREEN; 
const uint32_t COLOR_BORDE = TFT_DARKGREY;

const int GRID_ROWS = 3;
const int GRID_COLS = 3;
const int GAP_SIZE = 5;

int squareSize;
int boardStartX;
int boardStartY;

bool nueva_partida = 0;
bool ficha_colocada = 0;

bool boardState[GRID_ROWS][GRID_COLS];

// Definiciones para el teclado externo de la M5
#define KEYBOARD_I2C_ADDR 0x08
#define KEYBOARD_INT 5

// --- Declaraciones anticipadas de las funciones ---
void sysInfo();
void sysInfoPage1();
void sysInfoPage2();
void aboutM5ez();
void powerOff();
void adf_dif();
void setupBoard();
void drawSquare(int row, int col, uint32_t currentColor);
void drawBoard();
void calculateBoardDimensions();
void drawTurnoIndicator(uint16_t x, uint16_t w);
void redraw_board();
uint16_t getRandomColor();
void pantallaVictoriaAnimada();
void reproducirMelodiaVictoria();
void sonarNota(int frecuencia, int duracion_ms);
void reproducirMelodiaDerrota();
void pantallaDerrotaAnimada();
void reproducirMelodiaEmpate();
void pantallaEmpateAnimada();
void drawTurnBars();
void menucalculadora();

//Declaracion de la variable pantalla que actualiza loq ue ve el usuario
enum pantalla
{
  pantalla_principal,   
  pantalla_manual,      
  pantalla_auto,    
  pantalla_victoria,   
  pantalla_derrota,
  pantalla_empate,  
  pantalla_coor_abs,
};

enum introducir_coor
{
  coorx_in,
  coory_in,
  coorz_in,
};

introducir_coor coor_in = coorx_in;

pantalla pantalla_act = pantalla_principal;
resultado resul = sin_resultado;

void setup()
{
  Serial.begin(115200);
  mutex_setup(); // Se inicializa el mutex y se dejan 2 segundos para que todo se prepare
  cli.setTimeout(2000);
  delay(1000);
  xTaskCreate(mantener_wifi, "Mantiene la conexión WiFi activa", 20000, NULL, 6, NULL);
  xTaskCreate(recibir_mensaje, "Recibe e imprime por pantalla los datos enviados por el cliente", 20000, NULL, 5, NULL);

  // Inicializa M5ez. Esto también inicializa el hardware de M5Stack.
  ez.begin();
  Wire.begin(); // Inicializa el bus I2C

  //Permite poder obtener valores aleatorios
  randomSeed(analogRead(0));

  calculateBoardDimensions();
  setupBoard();

  pinMode(KEYBOARD_INT, INPUT_PULLUP);
  CalNum = 1;

  ezt::setDebug(INFO);

  //Primera pantalla al iniciar con el logotipo de mrcatronica incluido
  M5.Lcd.fillScreen(ez.theme->background);

  M5.Lcd.setRotation(1);

  // Calcular las coordenadas para centrar el logo
  int logo_width = 146; 
  int logo_height = 141;
  int logo_x = (M5.Lcd.width() - logo_width) / 2;
  int logo_y = 5;

  // Se dibuja el logo del master
  M5.Lcd.drawJpg(logo_mecatronica_jpg, logo_mecatronica_jpg_len, logo_x, logo_y);

  // Configuracion de la letra y el tamaño
  M5.Lcd.setFreeFont(&FreeSans18pt7b);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.setTextDatum(MC_DATUM); 

  M5.Lcd.drawString("Bienvenido", M5.Lcd.width() / 2, logo_y + logo_height + 35);
  M5.Lcd.drawString("Toca para iniciar", M5.Lcd.width() / 2, M5.Lcd.height() - 20);

  // Esperar a que el usuario presione un botón del M5Stack (A, B o C) antes de continuar al menú principal
  while (true)
  {
    M5.update(); 
    if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed())
    {
      M5.Speaker.tone(1000, 100); 
      break;                      
    }
    delay(50); 
  }
}

void loop()
{

  //Este switch case es el que maneja la pantalla actual que muestra al usuario
  switch (pantalla_act)
  {
  case pantalla_principal:
  {
    //Estas variables ezmenu de la libreria M5ez permiten crear los menus 
    ezMenu mainmenu("Bienvenido a la interfaz");
    mainmenu.txtSmall();
    mainmenu.addItem("Ajustes", ez.settings.menu);
    mainmenu.addItem("Seleccion dificultad", adf_dif);
    mainmenu.addItem("Calibrado");
    mainmenu.addItem("Manual");
    mainmenu.addItem("Jugar");
    mainmenu.upOnFirst("last|up");
    mainmenu.downOnLast("first|down");
    mainmenu.runOnce();
    if (mainmenu.pickName() == "Jugar" && modo_func == 2)
    {
      nueva_partida = 1;
      pantalla_act = pantalla_auto;
    }
    if (mainmenu.pickName() == "Manual" && modo_func == 1)
    {
      pantalla_act = pantalla_manual;
    }
    if (mainmenu.pickName() == "Manual" && modo_func != 1)
    {
      ez.msgBox("", "Seleccione el modo manual");
    }
    if (mainmenu.pickName() == "Jugar" && modo_func == 0)
    {
      ez.msgBox("", "Selecciona un modo");
    }
    if (mainmenu.pickName() == "Calibrado"){
      
    }
    break;
  }
  case pantalla_auto:
  //Pantalla de modo automatico en la que se muestra el tablero con las posiciones ocupadas por cada uno
  {
    drawTurnBars();
    M5.update();

    if (nueva_partida == 1)
    {
      nueva_partida = 0;
      resul = sin_resultado;
      turno_actual = casilla_jugador;
      enviar("CAMARA-INICIO\n");
      enviar("INTERFAZ-INICIO\n");
      setupBoard();
      drawBoard();
      drawTurnBars();
    }
    if (turno_actual == casilla_jugador && J_check == 1)
    {
      J_check = 0;
      enviar("CAMARA-TURNO_IA\n");
      enviar("INTERFAZ-TURNO_IA\n");
      turno_actual = casilla_robot;
      drawTurnBars();
    }
    if (turno_actual == casilla_robot && R_check == 1)
    {
      R_check = 0;
      enviar("INTERFAZ-TURNO_J\n");
      ez.header.draw("turnoIndicator");
      turno_actual = casilla_jugador;
      drawTurnBars();
    }
    if (new_tablero == 1){
      new_tablero = 0;
      update_tablero();
      redraw_board();
      drawTurnBars();
    }
    if (M5.BtnC.wasPressed())
    {
      turno_actual = casilla_jugador;
      R_check = 0;
      J_check = 0;
      new_tablero = 0;
      Tablero_act.jugador_col.clear();
      Tablero_act.jugador_fila.clear();
      Tablero_act.robot_col.clear();
      Tablero_act.robot_fila.clear();
      pantalla_act = pantalla_principal;
      enviar("CAMARA-REINCIO\n");
      enviar("INTERFAZ-REINICIO\n");
    }
    if (resul != sin_resultado)
    {
      turno_actual = casilla_jugador;
      R_check = 0;
      J_check = 0;
      new_tablero = 0;
      Tablero_act.jugador_col.clear();
      Tablero_act.jugador_fila.clear();
      Tablero_act.robot_col.clear();
      Tablero_act.robot_fila.clear();
      enviar("CAMARA-REINCIO\n");
      enviar("INTERFAZ-REINICIO\n");
      switch (resul)
      {
      case victoria:
        resul = sin_resultado;
        pantalla_act = pantalla_victoria;
        break;
      
      case derrota:
        resul = sin_resultado;
        pantalla_act = pantalla_derrota;
        break;
      
      case empate:
        resul = sin_resultado;
        pantalla_act = pantalla_empate;
        break;
    }
  }
    
    break;
  }
  case pantalla_manual:
  {
    //Pantalla de manejo manual que permite elegir entre coordenadas absolutas y coordenadas relativas
    ezMenu menu_manual("Elija un modo");
    menu_manual.txtSmall();
    menu_manual.addItem("Coordenadas Absolutas");
    menu_manual.addItem("Movimiento joystick");
    menu_manual.addItem("Salir");
    menu_manual.upOnFirst("last|up");
    menu_manual.downOnLast("first|down");
    menu_manual.runOnce();

    if (menu_manual.pickName() == "Coordenadas Absolutas"){
      pantalla_act = pantalla_coor_abs;
    }
    if (menu_manual.pickName() == "Salir"){
      pantalla_act = pantalla_principal;
    }
    break;
  }

  case pantalla_coor_abs:
  {
    //Pantalla para que el usuario pueda enviar al robot unas coordenadas tablero deseadas
    //Segun introduce als coordenadas se van actualizando en la cabecera del menu
    String titulo_abs = "X: " + coor_x +" Y: " + coor_y + " Z: " + coor_z;
    ezMenu menu_abs(titulo_abs);
    menu_abs.txtSmall();
    menu_abs.addItem("Editar coordenada x");
    menu_abs.addItem("Editar coordenada y");
    menu_abs.addItem("Editar coordenada z");
    menu_abs.addItem("Enviar Robot");
    menu_abs.addItem("Salir");
    menu_abs.upOnFirst("last|up");
    menu_abs.downOnLast("first|down");
    menu_abs.runOnce();

    if (menu_abs.pickName() == "Editar coordenada x")
    {
      coor_in = coorx_in;
      menucalculadora();
    }

    if (menu_abs.pickName() == "Editar coordenada y")
    {
      coor_in = coory_in;
      menucalculadora();
    }

    if (menu_abs.pickName() == "Editar coordenada z")
    {
      coor_in = coorz_in;
      menucalculadora();
    }

    if (menu_abs.pickName() == "Enviar Robot")
    {
      enviar(coor_x + "," + coor_y + "," + coor_z + "\n");
    }
    
    if (menu_abs.pickName() == "Salir")
    {
      pantalla_act = pantalla_manual;
    }

    break;
  }
  case pantalla_victoria:
  //Pantalla para victoria
  {
    pantallaVictoriaAnimada();
    break;
  }
  case pantalla_derrota:
  //Pantalla de derrota
  {
    pantallaDerrotaAnimada();
    break;
  }

  case pantalla_empate:
  //Pantalla de empate
  {
    pantallaEmpateAnimada();
    break;
  }
  }
}

void adf_dif()
{
  //Pantalla para ajustar la dificultad de la partida
  String disp_val;
  while (true)
  {
    if (dificultad == 2)
    {
      disp_val = "Dificil";
    }
    else if (dificultad == 1)
    {
      disp_val = "Medio";
    }
    else
    {
      disp_val = "Facil";
    }
    ez.msgBox("Nivel de dificultad: ", disp_val, "-#ok##+", false);
    ez.buttons.show("-#ok#+");
    String b = ez.buttons.wait();

    if (b == "-" && dificultad > 0)
    {
      dificultad--;
    }
    if (b == "+" && dificultad < 2)
    {
      dificultad++;
    }
    if (b == "ok")
    {
      break;
    }
  }
}

void powerOff()
{
  M5.Speaker.tone(400, 100); 
  M5.Power.deepSleep();
}

void aboutM5ez()
{
  M5.Speaker.tone(800, 50); 
  ez.msgBox("Acerca de M5ez", "M5ez fue escrito por | Rop Gonggrijp | | https://github.com/M5ez/M5ez");
  M5.Speaker.tone(800, 50); 
}

void sysInfo()
{
  M5.Speaker.tone(800, 50); 
  ez.header.show("Información del Sistema");
  sysInfoPage1();
  while (true)
  {
    String btn = ez.buttons.poll();
    if (btn == "up")
    {
      M5.Speaker.tone(750, 40); 
      sysInfoPage1();
    }
    if (btn == "down")
    {
      M5.Speaker.tone(750, 40); 
      sysInfoPage2();
    }
    if (btn == "select")
    {
      M5.Speaker.tone(800, 50); 
      break;
    }
  }
}

void sysInfoPage1()
{
  ez.canvas.clear();
  ez.canvas.font(&FreeSans9pt7b);
  ez.canvas.lmargin(10);
  ez.canvas.y(ez.canvas.top() + 10);
  ez.canvas.printf("Modelo de chip: %s\n", ESP.getChipModel());
  ez.canvas.printf("Velocidad de chip: %d Mhz\n", ESP.getCpuFreqMHz());
  ez.canvas.printf("Núcleos de chip: %d\n", ESP.getChipCores());
  ez.canvas.printf("Tamaño de Flash: %d M\n", ESP.getFlashChipSize() / 1048576);
  ez.canvas.printf("RAM libre: %d K\n", ESP.getFreeHeap() / 1024);
  ez.canvas.printf("RAM libre mínima: %d K\n", ESP.getMinFreeHeap() / 1024);
  ez.canvas.printf("Dirección MAC: %s\n", WiFi.macAddress().c_str());
  ez.buttons.show("arriba#salir#abajo"); 
}

void sysInfoPage2()
{
  ez.canvas.clear();
  ez.canvas.font(&FreeSans9pt7b);
  ez.canvas.lmargin(10);
  ez.canvas.y(ez.canvas.top() + 10);
  ez.canvas.printf("Versión de M5ez: %s\n", ez.version().c_str());
  ez.canvas.printf("Arduino Core: %s\n", ESP.getSdkVersion());
  ez.canvas.printf("Rotación de pantalla: %d\n", M5.Lcd.getRotation());
  ez.buttons.show("arriba#salir#abajo");
}

// Función para calcular las dimensiones y posiciones del tablero
void calculateBoardDimensions()
{
  int displayWidth = M5.Lcd.width();
  int displayHeight = M5.Lcd.height();

  // Calcular el tamaño máximo posible para los cuadrados
  // Consideramos el espacio total menos los gaps
  int maxSquareWidth = (displayWidth - (GRID_COLS - 1) * GAP_SIZE) / GRID_COLS;
  int maxSquareHeight = (displayHeight - 50 - (GRID_ROWS - 1) * GAP_SIZE) / GRID_ROWS;

  // Se usa el menor de los dos para asegurar cuadrados perfectos
  squareSize = std::min(maxSquareWidth, maxSquareHeight);

  int totalBoardWidth = GRID_COLS * squareSize + (GRID_COLS - 1) * GAP_SIZE;
  int totalBoardHeight = GRID_ROWS * squareSize + (GRID_ROWS - 1) * GAP_SIZE;

  boardStartX = (displayWidth - totalBoardWidth) / 2;
  boardStartY = (displayHeight - totalBoardHeight) / 2;

  if (boardStartX < 0)
    boardStartX = 0;
  if (boardStartY < 0)
    boardStartY = 0;
}

void redraw_board()
//Funcion para redibujar el tablero segun el turno del jugador
{
  for (int index = 0; index < sizeof(Tablero_act.jugador_col); index++)
  {
    drawSquare(Tablero_act.jugador_fila[index], Tablero_act.jugador_col[index], COLOR_OCUPADO_JUGADOR);
  }
  
  for (int index = 0; index < sizeof(Tablero_act.robot_col); index++)
  {
    drawSquare(Tablero_act.robot_fila[index], Tablero_act.robot_col[index], COLOR_OCUPADO_ROBOT);
  }

}


// Función para inicializar el estado del tablero (todos vacíos)
void setupBoard()
{
  for (int r = 0; r < GRID_ROWS; r++)
  {
    for (int c = 0; c < GRID_COLS; c++)
    {
      boardState[r][c] = false; 
    }
  }
}

// Función para dibujar todo el tablero en la pantalla al inicio
void drawBoard()
{
  M5.Lcd.fillScreen(ez.theme->background);
    for (int r = 0; r < GRID_ROWS; r++)
    {
      for (int c = 0; c < GRID_COLS; c++)
      {
        drawSquare(r, c, COLOR_VACIO);
      }
    }
}

//Funcion para colorear un cuadrado determinado en funcion de la fila y columna a la que pertenece
void drawSquare(int row, int col, uint32_t currentColor)
{
  int x = boardStartX + col * (squareSize + GAP_SIZE);
  int y = boardStartY + row * (squareSize + GAP_SIZE);

  M5.Lcd.fillRect(x, y, squareSize, squareSize, currentColor);
  M5.Lcd.drawRect(x, y, squareSize, squareSize, COLOR_BORDE);
}


//Dibuja un cuadrado de color rojo y verde en funcion de a quien le pertenezca el turno
void drawTurnoIndicator(uint16_t x, uint16_t w)
{
  int squareSize = ez.theme->header_height - 6;          
  int startY = (ez.theme->header_height - squareSize) / 2; 

  uint16_t squareColor;
  String turnText; 

  if (turno_actual == casilla_jugador)
  {
    squareColor = TFT_GREEN; 
    turnText = "Jugador";
  }
  else
  {
    squareColor = TFT_RED; 
    turnText = "Robot";
  }

  M5.Lcd.fillRect(x, 0, w, ez.theme->header_height, ez.theme->header_bgcolor);
  M5.Lcd.fillRect(x + 5, startY, squareSize, squareSize, squareColor);
  ez.setFont(ez.theme->header_font);             
  M5.Lcd.setTextColor(ez.theme->header_fgcolor);
  M5.Lcd.setTextDatum(ML_DATUM);   

  M5.Lcd.drawString(turnText, x + 5 + squareSize + 5, ez.theme->header_height / 2);
}

//Funcion para obtener un color aleatorio
uint16_t getRandomColor() {
    return M5.Lcd.color565(random(256), random(256), random(256));
}

//Funcion para animacion de victoria con confeti cayendo
void pantallaVictoriaAnimada() {
  M5.Lcd.fillScreen(TFT_BLACK);  

   M5.Lcd.setFreeFont(&FreeSansBold18pt7b);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_YELLOW);
    M5.Lcd.drawString("¡VICTORIA!", M5.Lcd.width() / 2, 120);

  static bool melodiaReproducida = false;
if (!melodiaReproducida) {
  reproducirMelodiaVictoria();
  melodiaReproducida = true;
}

  unsigned long startTime = millis();
  int confetiCount = 50;
  int confetiX[confetiCount];
  int confetiY[confetiCount];
  uint16_t confetiColor[confetiCount];

  for (int i = 0; i < confetiCount; i++) {
    confetiX[i] = random(0, M5.Lcd.width());
    confetiY[i] = random(-100, 0);
    confetiColor[i] = M5.Lcd.color565(random(50,255), random(50,255), random(50,255));
  }

  //Se reproduce hasta que el usuario pulsa el tercer boton principal del M5stack con el texto botando
  while (true) {
    M5.update();
    if (M5.BtnC.wasPressed()){
      melodiaReproducida = false;
      break; 
    }

    int bounceY = 120 + 10 * sin((millis() - startTime) / 200.0);
    M5.Lcd.setFreeFont(&FreeSansBold18pt7b);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_YELLOW);
    M5.Lcd.drawString("¡VICTORIA!", M5.Lcd.width() / 2, bounceY);

    for (int i = 0; i < confetiCount; i++) {
      M5.Lcd.fillCircle(confetiX[i], confetiY[i], 4, confetiColor[i]);
      confetiY[i] += random(2, 6); 

      if (confetiY[i] > M5.Lcd.height()) {
        confetiY[i] = random(-20, 0);
        confetiX[i] = random(0, M5.Lcd.width());
        confetiColor[i] = M5.Lcd.color565(random(50,255), random(50,255), random(50,255));
      }
    }

    delay(15);
    M5.Lcd.fillRect(0, 0, M5.Lcd.width(), M5.Lcd.height(), TFT_BLACK);
  }

  // Al salir, volver al menú principal
  pantalla_act = pantalla_principal;
}

void reproducirMelodiaVictoria() {
  sonarNota(988, 150); 
  sonarNota(1174, 150);
  sonarNota(1318, 200);
  sonarNota(1568, 150);  
  sonarNota(1760, 150); 
  sonarNota(1976, 250); 

  delay(80);

  sonarNota(2093, 500);  
}

void sonarNota(int frecuencia, int duracion_ms) {
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL); // Conecta pin y canal
  ledcWriteTone(BUZZER_CHANNEL, frecuencia); // Genera tono
  delay(duracion_ms);
  ledcWriteTone(BUZZER_CHANNEL, 0);          // Detiene tono
  delay(50); // Pausa entre notas
}

//Funcion para pantalla de derrota
void pantallaDerrotaAnimada() {
  M5.Lcd.fillScreen(TFT_BLACK); 
  static bool melodiaReproducida = false;
if (!melodiaReproducida) {
  reproducirMelodiaDerrota();
  melodiaReproducida = true;
}

  while (true) {
    M5.update();
    if (M5.BtnC.wasPressed()){
      melodiaReproducida = false;
      break;  
    }
    M5.Lcd.setFreeFont(&FreeSansBold18pt7b);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_RED);
    M5.Lcd.drawString("¡DERROTA!", M5.Lcd.width() / 2, 120);
  }

  // Al salir, volver al menú principal
  pantalla_act = pantalla_principal;
}

//Musica de derrota
void reproducirMelodiaDerrota() {
  sonarNota(784, 250);  
  sonarNota(659, 300);   
  sonarNota(587, 300);  

  delay(100);

  sonarNota(440, 400); 
  sonarNota(392, 500);  
}

//Musica de empate
void reproducirMelodiaEmpate() {
  sonarNota(659, 200);
  sonarNota(698, 200); 
  sonarNota(659, 200);  
  delay(100);
  sonarNota(523, 300); 
}

//Funcion para pantalla de empate
void pantallaEmpateAnimada() {
  M5.Lcd.fillScreen(TFT_DARKGREY);  

  static bool melodiaReproducida = false;
if (!melodiaReproducida) {
  reproducirMelodiaEmpate();
  melodiaReproducida = true;
}

  while (true) {
    M5.update();
    if (M5.BtnC.wasPressed()){
      melodiaReproducida = false;
      break;  // Salir al presionar botón C
    }
    M5.Lcd.setFreeFont(&FreeSansBold18pt7b);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_LIGHTGREY);
    M5.Lcd.drawString("¡EMPATE!", M5.Lcd.width() / 2, 120);
  }

  // Al salir, volver al menú principal
  pantalla_act = pantalla_principal;
}

//Funcion para dibujar las barras de turno
void drawTurnBars() {
  int barHeight = 12;  
  int screenWidth = M5.Lcd.width();
  int screenHeight = M5.Lcd.height();

  if (turno_actual == casilla_robot) {
    M5.Lcd.fillRect(0, 0, screenWidth, barHeight, RED);
    M5.Lcd.fillRect(0, screenHeight - barHeight, screenWidth, barHeight, ez.theme->background);
  } else if (turno_actual == casilla_jugador) {
    M5.Lcd.fillRect(0, screenHeight - barHeight, screenWidth, barHeight, GREEN);
    M5.Lcd.fillRect(0, 0, screenWidth, barHeight, ez.theme->background);
  }
}

//Funcion para leer el teclado y poder usar sus operadores
void menucalculadora() {
  ez.header.show("Introduzca la coordenada"); 
  ez.buttons.show("OK"); 
  ez.canvas.clear();            
  ez.canvas.font(&FreeSans12pt7b);   

  int yPosEntrada = 100;

  int textLineHeight = M5.Lcd.fontHeight() + 2;
  int textXStart = 10;
  int textWidth = M5.Lcd.width() - textXStart - 10; 

  Datain = ""; 
  bool redrawNeeded = true; 

  while (true) {
    // ---- Actualizar la visualización de la entrada
    if (redrawNeeded) {
      M5.Lcd.fillRect(textXStart, yPosEntrada, textWidth, textLineHeight, ez.theme->background);
      ez.canvas.pos(textXStart, yPosEntrada);
      ez.canvas.print("Entrada: " + Datain);
      redrawNeeded = false; 
    }

    // Verificar si hay una interrupción del teclado 
    if(digitalRead(KEYBOARD_INT) == LOW)
    {
      Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  

      while (Wire.available())
      {
        key_val = Wire.read();               
        if(key_val != 0) // Si se recibió una tecla válida 
        {
          if(isDigit(key_val) || key_val == '.')
          {
            Datain += (char)key_val; // Añadir el caracter a la cadena de entrada
            redrawNeeded = true; 
          }
          // Si el usuario pulsa 'A', reiniciar la entrada
          else if (key_val == 'A') {
            Datain = "";
            redrawNeeded = true; 
          }
        }
      }
    }

    String btn = ez.buttons.poll();
    if (btn == "OK") { // Si el botón "Atrás" es presionado, salir del bucle
      //Si el valor el superior o inferior a los margenes se usan estos valores frontera
      switch (coor_in)
      {
      case coorx_in:
          if(Datain.toFloat() < -70){
            coor_x = "-70.0";
          }
          else if(Datain.toFloat() > 70){
            coor_x = "70.0";
          }
          else{
            coor_x = Datain;
          }
        break;
      case coory_in:
        if(Datain.toFloat() < 200){
            coor_y = "200.0";
          }
          else if(Datain.toFloat() > 360.0){
            coor_y = "360.0";
          }
          else{
            coor_y = Datain;
          }
        break;
      case coorz_in:
          if(Datain.toFloat() < -20){
            coor_z = "-20.0";
          }
          else if(Datain.toFloat() > 120){
            coor_z = "120.0";
          }
          else{
            coor_z = Datain;
          }
        break;
      
      default:
        break;
      }
      ez.header.show(""); 
      ez.buttons.show(""); 
      ez.canvas.clear();  
      break;      
    }
    delay(50); 
  }
}