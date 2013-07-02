// Sender ID, Sibling ID, Side Number
String connectMessage = "+,04,14,2";
String disconnectMessage = "-,04,14,2";

void setup () {
  Serial.begin(9600);
}

void loop () {
  delay(1000);
  Serial.println(connectMessage);
}
