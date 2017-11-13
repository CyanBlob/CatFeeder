#include <ESP8266WiFi.h>

#define DELAY_TIME 60000
#define MIN_WEIGHT 1.0f
#define QUEUE_SIZE 60

// WiFi username and password
const char* ssid     = "Bluffs_Resident";
const char* password = "bluffsresident";

const char* URI = "/update?api_key=";
const char* API_KEY = "UFJUS014IUJ48JRB";
const char* TAIL = "&field1=";

const char* host = "api.thingspeak.com";
const int port = 80;

WiFiClient client;

void setup() 
{
  Serial.begin(115200);

  randomSeed(1);

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

// publish single datapoint to ThingSpeak
bool upload(float weight)
{
  if (!client.connect(host, port))
  {
      Serial.println("Failed to connect to host");
      return false;
  }

  // sprintf for floats hasn't been implemented on Arduino -_-
  /*char buf[10];
  sprintf(buf, "%f", weight);
  Serial.println(buf);*/

  Serial.println(String(weight, 2));

  // API call to upload weight
  client.print(String("GET ") + URI + API_KEY + TAIL + String(weight, 2) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(100);
  while (client.available())
  {
      String line = client.readStringUntil('\r');
      //Serial.println("RESPONSE: ");
      //Serial.println(line);
  }
  return true;
}

// upload all queued data
bool post_data(float weight[], int& queued)
{
  while (queued > 0)
  {
    Serial.print("UPLOADING VALUE: ");
    Serial.println(queued);
    if (upload(weight[queued - 1]))
    {
      --queued;
    }
    else
    {
      Serial.println("Upload failed");
      return false;
    }
  }
  return true;
}

float read_weight()
{
  return random(10);
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
  float weight_queue[QUEUE_SIZE];
  int queued = 0;

  while (true)
  {
    weight_queue[queued++] = read_weight();
    if (weight_queue[queued - 1] < MIN_WEIGHT || check_dispense())
    {
      dispense();
    }
    post_data(weight_queue, queued);
    // TODO: Subtract upload time from delay time
    delay(DELAY_TIME);
  }
}
