#include <WiFi.h>
#include <HTTPClient.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

const char* ssid = "yourNetworkName";
const char* password =  "yourPassword";
char* p = " ";
void setup() {

  Serial.begin(115200);
  delay(4000);
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

      Serial.println("Connecting to WiFi..");

      HTTPClient http;

      http.begin("http://demo4657392.mockable.io/list-tag-ids"); //Specify the URL
      int httpCode = http.GET();                                        //Make the request

      if (httpCode > 0) { //Check for the returning code

        
        String payload = http.getString();
        Serial.println(httpCode);
        int len = payload.length();
        //        p = payload.
        
        payload.toCharArray(p, len);
        writeFile(SD, "student_id/", p);
      }

      else {
        Serial.println("Error on HTTP request");
      }

      http.end(); //Free the resources
    }

  }

}

void loop() {
  readFile(SD, "/student_id");
}

void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
}


void writeFile(fs::FS & fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  writeFile(SD, "/hello.txt", "Hello ");


  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}
