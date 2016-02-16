#include "UbidotsWiFire.h"

/**
 * Constructor.
 */
Ubidots::Ubidots(char* token){
    _token = token;
    maxValues = 20;
    currentValue = 0;
    state = CONNECT;
    val = (Value *)malloc(maxValues * sizeof(Value));
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::parseValue(String body){
  String rawvalue;
  float num;
  char reply[15];
  uint8_t bodyPosinit;
  uint8_t bodyPosend;
  bodyPosinit = 4 + body.indexOf("\r\n\r\n");
  rawvalue = body.substring(bodyPosinit);
  bodyPosinit = 9 + rawvalue.indexOf("\"value\": ");
  bodyPosend = rawvalue.indexOf(", \"timestamp\"");
  rawvalue.substring(bodyPosinit,bodyPosend).toCharArray(reply, 10);
  num = atof(reply); 
  return num;
}
/** 
 * This function is to transform a String to Byte array and send it to state machine
 * @arg str the string that you want to transform
 * @return response the response of the API
 */
String Ubidots::stringToByteArrayAndGoToStateMachine(String str){
  String response;
  byte rgbWriteStream[str.length()];
  str.getBytes(rgbWriteStream, str.length() + 1);
  int cbWriteStream = sizeof(rgbWriteStream);
  response = stateMachine(rgbWriteStream, cbWriteStream);
  return response;
}
/** 
 * This function is to get the last value of variable from the Ubidots API
 * @arg id the id where you will get the data
 * @return value the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id){	
  float value;
  String raw;
  String message;
  char mess[300];
  sprintf(mess,"GET /api/v1.6/variables/%s/values?page_size=1 HTTP/1.1\nHost: things.ubidots.com\nUser-Agent: chipKitWiFire/1.0\nX-Auth-Token: %s\nConnection: close\n\n", id, _token);
	message = String(mess);  
  raw = stringToByteArrayAndGoToStateMachine(message);
  value = parseValue(raw); 
  return value;
  
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, double value){
  (val+currentValue)->id = variable_id;
  (val+currentValue)->value_id = value;
  currentValue++;
  if(currentValue>maxValues){
  	Serial.println("You are sending more than 20 consecutives variables, you just could send 5 variables. Then other variables will be deleted!");
  	currentValue = maxValues;
  }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll(){
  int i;
  char vals[10];
  char mess[2000];
  char data[1500];
  String message;  
  sprintf(data,"[");
  for(i=0; i<currentValue;){
    dtostrf((val + i)->value_id, 10, 3, vals);
    sprintf(data, "%s{\"variable\": \"%s\", \"value\":\"%s\"}", data, (val + i)->id, vals);
    i++;
    if(i<currentValue){
      sprintf(data,"%s, ", data);
    }
  }
  sprintf(data, "%s]", data);
  sprintf(mess,"POST /api/v1.6/collections/values/?force=true HTTP/1.1\nHost: things.ubidots.com\nUser-Agent: chipKitWiFire/1.0\nX-Auth-Token: %s\nConnection: close", _token);
  sprintf(mess,"%s\nContent-Type: application/json\nContent-Length:  %d\n\n%s\n",mess, strlen(data), data);
  message = String(mess);
  currentValue = 0;
  stringToByteArrayAndGoToStateMachine(message);
  return true;    
}
/**
 * Set the ssid and the pass of WiFi 
 * @arg ssid the ssid of the WiFi
 * @arg pass the pass of the WiFi
 */
void Ubidots::setWifiConnection(char* ssid, char* pass){
  szSsid = ssid;
  szPassPhrase = pass;
}
/** 
 * This function is a state machine without this chipKit doesn't func
 * @arg rgbWriteStream byte array to communicate with the Ubidots API
 * @arg cbWriteStream size of rgbWriteStream
 * @return response of the API
 */
String Ubidots::stateMachine(byte rgbWriteStream[], int cbWriteStream){
  String response;
  int i = 0;
  int flag = 0;
  unsigned tStart = 0;
  unsigned tWait = 3000;
  for (;;)
  {
    
    int cbRead = 0;
    IPSTATUS status;
    switch (state)
    {

      case CONNECT:
        
        if (WiFiConnectMacro())
        {
          Serial.println("WiFi connected");
          deIPcK.begin();
          flag = 0;
          state = TCPCONNECT;
        }
        else if (IsIPStatusAnError(status)){
          Serial.print("Unable to connection, status: ");
          Serial.println(status, DEC);
          flag = 0;
          state = CLOSE;
        }else{
          flag++;
          delay(10);
        }
        if(flag>1000){          
          deIPcK.end();
          deIPcK.DEWFcK::wfDisconnect();
          flag = 0;
          state = CONNECT;
          
        }
        break;

      case TCPCONNECT:
        
        if(deIPcK.tcpConnect(WEBSITE, PORT, tcpSocket))
        {
          Serial.println("Connected to server.");
          state = WRITE;
          flag = 0;
          break;
        }else {
          delay(1);
          flag++;
        }
        if(flag>10000){
          deIPcK.DEWFcK::wfDisconnect();
          state = CONNECT;
          flag = 0;
        }
        break;

      case WRITE:
        if (tcpSocket.isEstablished())
        {
          Serial.println("Sending request");
          Serial.write(rgbWriteStream, cbWriteStream);
          tcpSocket.writeStream(rgbWriteStream, cbWriteStream);
          state = READ;
          tStart = (unsigned) millis();
          Serial.println("Reading data");
        }
        break;


      case READ:

        char c;
        while ((cbRead = tcpSocket.available()) > 0)
        {
           
              c = tcpSocket.readByte();
              response += c;  
           i++;          
        }
        if ( (((unsigned) millis()) - tStart) > tWait ){
          Serial.println("");
          response[i]='\0';
          state = CLOSE;
        }
        break;


      case CLOSE:
        tcpSocket.close();
        state = DONE;
        break;

      case DONE:
        if(deIPcK.DEWFcK::isWFInitialized()){
          state = TCPCONNECT;  
        }else{
          state = CONNECT;
          deIPcK.end();
        }
        
        DEIPcK::periodicTasks();
        return response;

      default:
        break;

    }

    DEIPcK::periodicTasks();
    // keep the stack alive everytime it passes through the loop()
  }
}