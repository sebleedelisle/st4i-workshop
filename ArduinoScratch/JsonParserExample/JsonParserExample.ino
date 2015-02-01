/*
* Arduino JSON library - Parser Example
* Benoit Blanchon 2014 - MIT License
*/

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

void setup()
{
    Serial.begin(115200);

    char json [] = "{\n  \"message\": \"success\", \n  \"request\": {\n    \"altitude\": 100, \n    \"latitude\": 50.8321, \n    \"passes\": 2, \n    \"longitude\": -0.2075, \n    \"datetime\": 1413989979\n  }, \n  \"response\": [\n    {\n      \"duration\": 646, \n      \"risetime\": 1413992852\n    }, \n    {\n      \"duration\": 651, \n      \"risetime\": 1413998660\n    }\n  ]\n}";
    JsonParser<64> parser;
    Serial.println(json); 
    JsonObject root = parser.parse(json);

    if (!root.success())
    {
        Serial.println("JsonParser.parse() failed");
        return;
    }

    Serial.println("Success!"); 
    //Serial.println(root); 
    long  risetime    = root["response"][0]["risetime"];
    
    Serial.println(risetime); 
//    long   time      = root["time"];
//    double latitude  = root["response"][0];
//    double longitude = root["data"][1];
//
//    Serial.println(sensor);
//    Serial.println(time);
//    Serial.println(latitude, 6);
//    Serial.println(longitude, 6);
}

void loop()
{

}
