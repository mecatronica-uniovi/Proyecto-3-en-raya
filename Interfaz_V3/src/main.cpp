#include <WiFiClient.h>
#include <MyServer.h>

#define PLO1 4
#define PLO2 5
#define PLO3 6
#define SEL1 9
#define SEL2 10
#define PSH 11
#define JOY1 8
#define JOY2 7

// Variables de parpadeo
unsigned long prev_time1 = 0, prev_time2 = 0;
bool state1 = false, state2 = false;
int retardo1 = 500, retardo2 = 500;
int estado_sel=0;
int estado_sel_ant = 0;

// Bandera activada por la interrupción
volatile bool psh_pressed = false;

// ISR del botón
void IRAM_ATTR handleButtonInterrupt() 
{
  psh_pressed = true; // solo marca evento
}

// Función para controlar LEDs según el estado del juego
void actualizarLuces() 
{
  // MAIN: todo apagado
  if (estado_sel == 0) 
  {
    digitalWrite(PLO1, LOW);  // Verde
    digitalWrite(PLO2, LOW);  // Amarillo
    digitalWrite(PLO3, LOW);  // Rojo
    return;
  }

  // MANUAL: solo amarillo
  if (estado_sel == 1) 
  {
    digitalWrite(PLO1, LOW);
    digitalWrite(PLO2, HIGH);
    digitalWrite(PLO3, LOW);
    return;
  }

  // AUTO: verde siempre
  if (estado_sel == 2) 
  {
    digitalWrite(PLO1, HIGH);
    digitalWrite(PLO2, LOW);

    // Turno de IA: prende el rojo
    if (turno_ia == 1) 
    {
      turno_ia=0;
      digitalWrite(PLO3, HIGH);

    } else if (turno_j==1)
    {
      turno_j=0;
      digitalWrite(PLO3, LOW);
    }
    return;
  }
}

void setup() 
{
  Serial.begin(115200);
  mutex_setup(); //Se inicializa el mutex y se dejan 2 segundos para que todo se prepare
  cli.setTimeout(2000);
  delay(1000);
  xTaskCreate(mantener_wifi,"Mantiene la conexión WiFi activa",20000,NULL,5,NULL);
  xTaskCreate(recibir_mensaje,"Recibe e imprime por pantalla los datos enviados por el cliente",20000,NULL,5,NULL);

  pinMode(PLO1, OUTPUT);
  pinMode(PLO2, OUTPUT);
  pinMode(PLO3, OUTPUT);
  pinMode(SEL1, INPUT);
  pinMode(SEL2, INPUT);
  //pinMode(JOY1, INPUT);
  //pinMode(JOY2, INPUT);
  pinMode(PSH, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PSH), handleButtonInterrupt, FALLING);
}

void loop() 
{
  unsigned long now = millis();

  // Leer SEL1 y SEL2
  int sel1_state = digitalRead(SEL1);
  int sel2_state = digitalRead(SEL2);

  // Leer joysticks y mapear a retardo
  //retardo1 = map(analogRead(JOY1), 0, 4095, 50, 1000);
  //retardo2 = map(analogRead(JOY2), 0, 4095, 50, 1000);

  // Procesar pulsación (marcada por la interrupción)
  if (psh_pressed) 
  {
    psh_pressed = false; // limpiar bandera
    enviar("M5-PULSADOR\n");
  }

  // Se cambia de estado en función de la posición del selector
  if (sel1_state == 1 && sel2_state == 0)
  {
    estado_sel = 1;
  }
  else if( sel1_state == 0 && sel2_state == 1)
  {
    estado_sel = 2;
  }
  else
  {
    estado_sel = 0;
  }

  // En función del estado se cambia a MAIN, MANUAL o AUTO
  if (estado_sel != estado_sel_ant)
  {
    if (estado_sel == 0)
    {
      enviar("M5-MAIN\n");       // Llama a la función deseada
    }
    else if (estado_sel == 1)
    {
      enviar("M5-MANUAL\n");       // Llama a la función deseada
    }
    else if (estado_sel == 2)
    {
      enviar("M5-AUTO\n");     // Llama a la función deseada
    }
    estado_sel_ant = estado_sel;
  }

  // Reinicio de partida
  if (reinicio == 1) 
  {
    reinicio = 0;
    inicio_luces = 0;
    turno_j = 0;
    turno_ia = 0;
  }

  // Se manejan los LEDs
  actualizarLuces();
}