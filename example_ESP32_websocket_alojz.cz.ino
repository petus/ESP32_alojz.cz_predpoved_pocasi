#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "MY_SSID"; //SSID WiFi
const char* password = "MY_PASSWORD"; //heslo WiFi
const char* host = "alojz.cz"; //webserver
String url = F("/praha"); //mesto
WiFiClientSecure client;

const char* root_ca PROGMEM = \
                              "-----BEGIN CERTIFICATE-----\n" \
                              "MIIFjDCCA3SgAwIBAgINAgCOsgIzNmWLZM3bmzANBgkqhkiG9w0BAQsFADBHMQsw\n"\
                              "CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n"\
                              "MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMjAwODEzMDAwMDQyWhcNMjcwOTMwMDAw\n"\
                              "MDQyWjBGMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n"\
                              "Y2VzIExMQzETMBEGA1UEAxMKR1RTIENBIDFENDCCASIwDQYJKoZIhvcNAQEBBQAD\n"\
                              "ggEPADCCAQoCggEBAKvAqqPCE27l0w9zC8dTPIE89bA+xTmDaG7y7VfQ4c+mOWhl\n"\
                              "UebUQpK0yv2r678RJExK0HWDjeq+nLIHN1Em5j6rARZixmyRSjhIR0KOQPGBMUld\n"\
                              "saztIIJ7O0g/82qj/vGDl//3t4tTqxiRhLQnTLXJdeB+2DhkdU6IIgx6wN7E5NcU\n"\
                              "H3Rcsejcqj8p5Sj19vBm6i1FhqLGymhMFroWVUGO3xtIH91dsgy4eFKcfKVLWK3o\n"\
                              "2190Q0Lm/SiKmLbRJ5Au4y1euFJm2JM9eB84Fkqa3ivrXWUeVtye0CQdKvsY2Fka\n"\
                              "zvxtxvusLJzLWYHk55zcRAacDA2SeEtBbQfD1qsCAwEAAaOCAXYwggFyMA4GA1Ud\n"\
                              "DwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwEgYDVR0T\n"\
                              "AQH/BAgwBgEB/wIBADAdBgNVHQ4EFgQUJeIYDrJXkZQq5dRdhpCD3lOzuJIwHwYD\n"\
                              "VR0jBBgwFoAU5K8rJnEaK0gnhS9SZizv8IkTcT4waAYIKwYBBQUHAQEEXDBaMCYG\n"\
                              "CCsGAQUFBzABhhpodHRwOi8vb2NzcC5wa2kuZ29vZy9ndHNyMTAwBggrBgEFBQcw\n"\
                              "AoYkaHR0cDovL3BraS5nb29nL3JlcG8vY2VydHMvZ3RzcjEuZGVyMDQGA1UdHwQt\n"\
                              "MCswKaAnoCWGI2h0dHA6Ly9jcmwucGtpLmdvb2cvZ3RzcjEvZ3RzcjEuY3JsME0G\n"\
                              "A1UdIARGMEQwCAYGZ4EMAQIBMDgGCisGAQQB1nkCBQMwKjAoBggrBgEFBQcCARYc\n"\
                              "aHR0cHM6Ly9wa2kuZ29vZy9yZXBvc2l0b3J5LzANBgkqhkiG9w0BAQsFAAOCAgEA\n"\
                              "IVToy24jwXUr0rAPc924vuSVbKQuYw3nLflLfLh5AYWEeVl/Du18QAWUMdcJ6o/q\n"\
                              "FZbhXkBH0PNcw97thaf2BeoDYY9Ck/b+UGluhx06zd4EBf7H9P84nnrwpR+4GBDZ\n"\
                              "K+Xh3I0tqJy2rgOqNDflr5IMQ8ZTWA3yltakzSBKZ6XpF0PpqyCRvp/NCGv2KX2T\n"\
                              "uPCJvscp1/m2pVTtyBjYPRQ+QuCQGAJKjtN7R5DFrfTqMWvYgVlpCJBkwlu7+7KY\n"\
                              "3cTIfzE7cmALskMKNLuDz+RzCcsYTsVaU7Vp3xL60OYhqFkuAOOxDZ6pHOj9+OJm\n"\
                              "YgPmOT4X3+7L51fXJyRH9KfLRP6nT31D5nmsGAOgZ26/8T9hsBW1uo9ju5fZLZXV\n"\
                              "VS5H0HyIBMEKyGMIPhFWrlt/hFS28N1zaKI0ZBGD3gYgDLbiDT9fGXstpk+Fmc4o\n"\
                              "lVlWPzXe81vdoEnFbr5M272HdgJWo+WhT9BYM0Ji+wdVmnRffXgloEoluTNcWzc4\n"\
                              "1dFpgJu8fF3LG0gl2ibSYiCi9a6hvU0TppjJyIWXhkJTcMJlPrWx1VytEUGrX2l0\n"\
                              "JDwRjW/656r0KVB02xHRKvm2ZKI03TglLIpmVCK3kBKkKNpBNkFt8rhafcCKOb9J\n"\
                              "x/9tpNFlQTl7B39rJlJWkR17QnZqVptFePFORoZmFzM=\n"\
                              "-----END CERTIFICATE-----\n";

void setup() {
  Serial.begin(115200); //rychlost seriovej linky
  Serial.println();
  Serial.print(F("Pripajanie na WiFi siet: "));
  Serial.println(ssid);
  WiFi.begin(ssid, password); //pripoj sa na wifi siet s heslom

  while (WiFi.status() != WL_CONNECTED) { //pokial sa nepripojime na wifi opakuj pripajanie a spustaj funkcie pre ovladanie v offline rezime
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print(F("Wifi pripojene s IP: "));
  Serial.println(WiFi.localIP());
  client.setCACert(root_ca);
  if (client.connect(host, 443)) {
    Serial.println(F("Uspesne pripojenie na webserver"));
    client.print(String("GET ") + url + " HTTP/1.0\r\n" + "Host: " + host + "\r\n" + "User-Agent: ESP32\r\n" + "Connection: close\r\n\r\n");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line.indexOf("<img class=\"alojz-to-left\" src") > 0) {
        line = client.readStringUntil('\n');
        Serial.print(F("Predpoved je pre den a miesto: "));
        Serial.println(line);
      }
      if (line.indexOf("<h2 class=\"actual-forecast\">") > 0) {
        line = client.readStringUntil('\n');
        Serial.print(F("Aktualna predpoved: "));
        Serial.println(line);
      }
      if (line.indexOf("<span class=\"actual-forecast-day2\">") > 0) {
        line = client.readStringUntil('\n');
        Serial.print(F("Predpoved na zajtra: "));
        Serial.println(line);
        break;
      }
    }
  } else {
    Serial.println(F("Pripojenie na webserver zlyhalo..."));
  }
  client.stop();
}

void loop() {
  // put your main code here, to run repeatedly:

}
