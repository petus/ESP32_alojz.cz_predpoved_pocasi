/*
*
* Priklad pouziti ESP32 a stahovani informace o pocasi 
* z alojz.cz
* Splacano na kolene.
*
* Kod vytvori sifrovane spojeni se strankou, stahne zdrojovy kod stranky,
* najde parametry <h2> a </h2> a text mezi posle na vystup.
*
* Vytvoril chiptron.cz (2021)
*
*/

// klik na zamecek, zabezpecene spojeni, vice informaci, zobrazit certifikat, 
// GTS CA 1D4, stahnout PEM certifikat, text zkopirovat
const char* root_ca= \
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

#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "SSID";
const char* password =  "PASSWORD";
 
void setup() {
 
  Serial.begin(115200);
  delay(1000);
 
  WiFi.begin(ssid, password); 
 
  Serial.println("Connecting to WiFi.");

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println(".");
  }
 
  Serial.println("Connected to the WiFi network");
}
 
void loop() {
  String payload;
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;

    //beroun, brno, olomouc.. je na vyber hodne mest
    http.begin("https://www.alojz.cz/praha", root_ca); //Specify the URL and certificate
    int httpCode = http.GET();                                                  //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }

  int firstClosingBracket = payload.indexOf("<h2");
  int lastClosingBracket = payload.indexOf("</h2>", firstClosingBracket + 1 );

  Serial.println(payload.substring(firstClosingBracket+49, lastClosingBracket)); // pozice od <h2 + mezery
  Serial.println(fixCzechCharacters(payload.substring(firstClosingBracket+49, lastClosingBracket)));
 
  delay(60000);
}

// preuzito z https://navody.dratek.cz/arduino-projekty/notifikace-prichoziho-emailu-s-esp8266.html
String fixCzechCharacters(String data) {
	data.replace("Ě", "E");
	data.replace("Š", "S");
	data.replace("Č", "C");
	data.replace("Ř", "R");
	data.replace("Ž", "Z");
	data.replace("Ý", "Y");
	data.replace("Á", "A");
	data.replace("Í", "I");
	data.replace("É", "E");
	data.replace("Ů", "U");
	data.replace("Ú", "U");
	data.replace("Ď", "D");
	data.replace("Ť", "T");
	data.replace("Ň", "N");
	data.replace("ě", "e");
	data.replace("š", "s");
	data.replace("č", "c");
	data.replace("ř", "r");
	data.replace("ž", "z");
	data.replace("ý", "y");
	data.replace("á", "a");
	data.replace("í", "i");
	data.replace("é", "e");
	data.replace("ů", "u");
	data.replace("ú", "u");
	data.replace("ď", "d");
	data.replace("ť", "t");
	data.replace("ň", "n");
	return data;
}
