// Definition der LED-Pins
const int numLeds = 7;
int ledPins[] = {10, 9, 8, 7, }; // LEDs sind an die Pins 6 - 7 angeschlossen
 
// Definition des Taster-Pins
const int buttonPin = 9; // Taster ist an Pin 9 angeschlossen
 
// Variablen für Taster- und Modusverwaltung
int currentMode = 0;       // Aktueller Leuchtmodus (0, 1, 2...)
int lastButtonState = LOW; // Vorheriger Zustand des Tasters
long lastDebounceTime = 0; // Zeit der letzten Zustandsänderung des Tasters
long debounceDelay = 50;   // Wartezeit zur Entprellung des Tasters
 
void setup() {
  // Alle LED-Pins als Ausgänge einstellen
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
 
  // Taster-Pin als Eingang einstellen
  // Man könnte INPUT_PULLUP anstelle eines externen Pull-Down-Widerstands verwenden,
  // dann würde die andere Seite des Tasters an GND angeschlossen. Zur besseren
  // Übersichtlichkeit bleiben wir jedoch beim externen Pull-Down-Widerstand.
  pinMode(buttonPin, INPUT);
 
  // Optional: Initialisierung des seriellen Monitors zum Debuggen
  // Serial.begin(9600);
}
 
// --- Funktionen zur Steuerung der LEDs ---
 
// Funktion zum Ausschalten aller LEDs
void turnAllLedsOff() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}
 
// Modus 0: Lauflicht (Knight Rider)
void modeKnightRider() {
  // Bewegung in eine Richtung
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH); // Aktuelle LED einschalten
    delay(100);                    // 100 Millisekunden warten
    digitalWrite(ledPins[i], LOW);  // Aktuelle LED ausschalten
    // Taster während der Animation überprüfen, um schnell auf Moduswechsel zu reagieren
    if (checkButtonForModeChange()) return;
  }
 
  // Bewegung in die andere Richtung (Rücklauf)
  // Beginnt bei der vorletzten LED (Index numLeds - 2)
  // und geht abwärts bis zur zweiten LED (Index 1), um doppelte Extrem-LEDs zu vermeiden
  for (int i = numLeds - 2; i >= 1; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
    if (checkButtonForModeChange()) return;
  }
}
 
// Modus 1: Alle LEDs blinken gleichzeitig
void modeAllFlash() {
  for (int j = 0; j < 3; j++) { // Blinkt 3 Mal
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    delay(300);
    if (checkButtonForModeChange()) return; // Taster überprüfen
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(300);
    if (checkButtonForModeChange()) return; // Taster überprüfen
  }
  delay(500); // Kleine Pause nach dem Zyklus
}
 
// Modus 2: Von der Mitte nach außen
void modeCenterOut() {
  for (int i = 0; i < (numLeds + 1) / 2; i++) { // Iteriert zur Mitte (oder etwas darüber hinaus)
    digitalWrite(ledPins[numLeds / 2 - i], HIGH); // Linke Seite von der Mitte
    if (numLeds / 2 + i < numLeds) { // Sicherstellen, dass der Index nicht außerhalb des Bereichs liegt
      digitalWrite(ledPins[numLeds / 2 + i], HIGH); // Rechte Seite von der Mitte
    }
    delay(150);
    if (checkButtonForModeChange()) return;
  }
  delay(500); // Kurz leuchten lassen
  turnAllLedsOff();
  delay(500);
}
 
// Funktion, die den Tasterstatus überprüft und den Modus wechselt
// Gibt true zurück, wenn der Modus gewechselt wurde, um die aktuelle Animation zu unterbrechen
boolean checkButtonForModeChange() {
  int reading = digitalRead(buttonPin);
 
  // Entprellung des Tasters
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
 
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != lastButtonState) {
      lastButtonState = reading;
      if (lastButtonState == HIGH) { // Taster wurde gedrückt (Zustand HIGH)
        currentMode++; // Modusnummer erhöhen
        if (currentMode >= 3) { // Wenn der letzte Modus überschritten wurde, auf 0 zurücksetzen
          currentMode = 0;
        }
        turnAllLedsOff(); // Alle LEDs ausschalten, bevor der Modus gewechselt wird
        // Serial.print("Modus gewechselt zu: "); // Debugging
        // Serial.println(currentMode); // Debugging
        return true; // Signalisiert, dass der Modus gewechselt wurde
      }
    }
  }
  lastButtonState = reading; // Aktuellen Tasterzustand speichern
  return false; // Modus wurde nicht gewechselt
}
 
 
void loop() {
  // Den Taster am Anfang jeder loop-Iteration überprüfen,
  // aber die Hauptkontrolle erfolgt in der Funktion checkButtonForModeChange
  // innerhalb der Modusfunktionen.
 
  switch (currentMode) {
    case 0:
      modeKnightRider();
      break;
    case 1:
      modeAllFlash();
      break;
    case 2:
      modeCenterOut();
      break;
    // Weitere Fälle für zusätzliche Modi hinzufügen
    default:
      // Standardmodus oder Reset
      currentMode = 0;
      break;
  }
}
