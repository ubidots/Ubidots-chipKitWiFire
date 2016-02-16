#include <MRF24G.h>
#include <UbidotsWiFire.h>

#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define ID2 "Your_variable_ID2_here"
#define ID3 "Your_variable_ID3_here"
#define ID4 "Your_variable_ID4_here"

#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define SSIDWIFI "Your_WiFi_SSID_here"  // Put here your WiFi SSID
#define PASS "Your_WiFi_PASS_here"  // Put here your WiFi password

Ubidots ubidots(TOKEN);

void setup(){
    Serial.begin(9600);
    client.setWifiConnection(SSIDWIFI, PASS);
}
void loop(){
    float value = analogRead(A12);
    float value2 = analogRead(A0);
    float value3 = analogRead(A1);
    float value4 = analogRead(A2);
    client.add(ID, value);
    client.add(ID2, value2);
    client.add(ID3, value3);
    client.add(ID4, value4);
    client.sendAll();
}
