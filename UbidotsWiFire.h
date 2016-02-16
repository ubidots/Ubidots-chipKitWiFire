#ifndef __UbidotsWiFire_
#define __UbidotsWiFire_
#define DEBUG_UBIDOTS

#include <DEIPcK.h>
#include <DEWFcK.h>

#define PORT 80
#define WEBSITE "things.ubidots.com"
#define WiFiConnectMacro() deIPcK.wfConnect(szSsid, szPassPhrase)

typedef struct Value {
  char  *id;
  float value_id;
} Value;

typedef enum{

  NONE = 0,
  CONNECT,
  TCPCONNECT,
  WRITE,
  READ,
  CLOSE,
  DONE,
} STATE;

class Ubidots{
    private:
        TCPSocket tcpSocket;
        char* _token;
        uint8_t maxValues;
        uint8_t currentValue;
        Value * val;
        char * szSsid;
        char * szPassPhrase;
        float parseValue(String body);
        String stateMachine(byte rgbWriteStream[], int cbWriteStream);
        String stringToByteArrayAndGoToStateMachine(String str);
        STATE state;    

    public:

        Ubidots(char* token);
        void add(char *variable_id, double value);
        void setWifiConnection(char* ssid, char* pass);
        float getValue(char* id);
        bool sendAll();        
};
#endif