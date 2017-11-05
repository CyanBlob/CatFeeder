#include <ESP8266WiFi.h>

#define DELAY_TIME 60000
#define MIN_WEIGHT 1.0f
#define QUEUE_SIZE 60

// WiFi username and password
const char* ssid     = "Bluffs_Resident";
const char* password = "bluffsresident";

const char* host = "";
const int port = 13;

float weight_queue[QUEUE_SIZE];
int queued = 0;

WiFiClient client;

void setup() 
{
  Serial.begin(115200);

  // TODO: connect to WiFi the flexible way (https://github.com/tzapu/WiFiManager)
  WiFi.begin(ssid, password);

  Serial.println("Wifi started");

  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
}

String get_timestamp()
{
  return "";
}

bool post_data(float weight[])
{
  return false;
}

float read_weight()
{
  return 0.0f;
}

// if the server is telling us to dispense
bool check_dispense()
{
  return false;
}

void dispense()
{
}

void loop() 
{
  weight_queue[queued++] = read_weight();
  if (weight_queue[queued - 1] < MIN_WEIGHT || check_dispense())
  {
    dispense();
  }
  post_data(weight_queue);
  delay(DELAY_TIME);
}
