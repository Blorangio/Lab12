#include "MQTT.h"
#include "oled-wing-adafruit.h"

#define LEDPin D5

SYSTEM_THREAD(ENABLED);

void callback(char *topic, byte *payload, unsigned int length);

MQTT client("lab.thewcl.com", 1883, callback);

OledWingAdafruit display;

void setup()
{
  Serial.begin(9600);
  client.subscribe("Iot2023");
  pinMode(LEDPin, OUTPUT);
  display.setup();
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}
void loop()
{
  display.loop();
  if (client.isConnected())
  {
    client.loop();
  }
  else
  {
    client.connect(System.deviceID());
    client.subscribe("Iot2023");
  }
  if (display.pressedA())
  {
    client.publish("Iot2023", "Button A Was Pressed");
  }
  if (display.pressedB())
  {
    client.publish("Iot2023", "Button B Was Pressed");
  }
  if (display.pressedC())
  {
    client.publish("Iot2023", "Button C Was Pressed");
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  Serial.printf("%s", p);
  Serial.println();

  if (p[0] == 'B')
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(p);
    display.display();
  } else {
    int receivedInt = atoi(p);

    if (receivedInt == 0)
    {
      digitalWrite(LEDPin, LOW);
    }
    else if (receivedInt == 1)
    {
      digitalWrite(LEDPin, HIGH);
    }
  }
}