float Inch = 0.00;
float cm = 0.00;
int SonarPin = A0;
int sensorValue;

int dataBuffer = 500;
int data[500];
int cmSoft; // valor suavizado;
int count = 0;


void setup() {
  pinMode(SonarPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  readSonar();
  // softenData();
  
  Serial.print(cm);
  Serial.print(" cm.-  ");
  Serial.println(millis());
}

void readSonar() {
  sensorValue = analogRead(SonarPin);
  delay(10);
  Inch = (sensorValue * 0.497);
  cm = Inch * 2.54;
}

void softenData() {
  data[count] = cm;
  count ++;
  count %= dataBuffer;
  long val = 0;
  for (int i = 0; i < dataBuffer; i++) {
    val += data[i];
  }
  cmSoft = val / dataBuffer;
}

