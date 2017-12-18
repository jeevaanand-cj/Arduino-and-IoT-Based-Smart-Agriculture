# Arduino-and-IoT-Based-Smart-Agriculture
#include <SoftwareSerial.h>
SoftwareSerial myGsm(7,8);
int led1 = 13;
String Data = "";
char num;
String number="";
int count=0;
int add=0;
int led2 = 11;
int led3 = 4;
int resetPin = 8;
int i= 1;
String Data1 = "";
bool flag = false;
bool flag1 = false;
bool flag2 = false;


void setup()
{
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  myGsm.begin(9600);  
  Serial.begin(9600);  
  digitalWrite(resetPin, HIGH);
  delay(200);  
  pinMode(resetPin, OUTPUT);
  Serial.println("reset");        
  myGsm.println("AT+CMGF=1");
  myGsm.println("AT+CNMI=2,2,0,0,0\r");
  myGsm.println("AT+CMGD=1,4");
}
void loop(){
if (myGsm.available()>0) {
      sms();
      char character = myGsm.read();
      if(character =='9'|| flag){ 
              Data = Data + character;
              add++;
              flag = true;
             }     
            else{
              Data = "";
            }
       if (add==12){
            Serial.print(Data);
            Serial.println();
            Serial.write("ring");
            count++;
            Serial.println(count);
            delay(100);  
             flag = false;          
            if(count==2){
              digitalWrite(led1,HIGH);
              Serial.println("motor on");
              myGsm.println("ATH0");
              Serial.println("Hang up......");
              delay(100);
              check();
              myGsm.println("AT+HTTPPARA=\"URL\",\"ur ip/ur url"\"");
              delay(1000);
              check_1();
              Data = "";                                                                              
          }else if(count>=3){
              Serial.println("motor off");
              digitalWrite(led1,LOW);
              myGsm.println("ATH0");
              Serial.println("Hang up.");
              delay(100);
              check();
              myGsm.println("AT+HTTPPARA=\"URL\",\"ur ip/ur url"\"");
              delay(1000);
              check_1();
              Data="";
              count = 0;
          } 
        add = 0;
       }
   }
 }
void check(){
              myGsm.println();
              myGsm.println("AT");
              myGsm.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");//setting the SAPBR,connection type is GPRS
              myGsm.println("AT+SAPBR=3,1,\"APN\",\"\"");//setting the APN,2nd parameter empty works for all networks 
              myGsm.println("AT+SAPBR=1,1");
              myGsm.println("AT+SAPBR=2,1");
              delay(1000);
              myGsm.println("AT+HTTPINIT"); //init the HTTP request
              myGsm.println("AT+HTTPPARA=\"CID\",1");
}
void check_1(){
              myGsm.println();
              myGsm.println("AT+HTTPACTION=0");//submit the GET request 
              delay(5000);//the delay is important if the return datas are very large, the time required longer.
              myGsm.println("AT+HTTPREAD=0,20");// read the data from the website you access
              delay(3000);
              myGsm.println("");
              delay(1000);
              myGsm.println("AT+HTTPTERM");// terminate HTTP service
              delay(1000); 
}
void sms(){
  char ch = myGsm.read();
  if(ch =='l'|| flag1 || ch =='L'){
          Data1.concat(ch);
          flag1 = true;
          }
  if(Data1 =="led on" || Data1 =="Led on"){
            Serial.println(Data1);
            digitalWrite(led2,HIGH);
            digitalWrite(led3,HIGH);
            Data1 ="";
            flag1 = false;
         }
  else if(Data1 =="led off" || Data1 =="Led off"){
            Serial.println(Data1);
            digitalWrite(led2,LOW);
            digitalWrite(led3,LOW);
            Data1 ="";
            flag1 = false;
          }
 else if(Data1 =="led time " || Data1 =="Led time " || flag2 ){
            flag2 = true;
                      if( i == 1 ){
                          i++;
                        }else{
                          flag2 = false;
                          flag1 = false;
                          while(1){
                              Serial.println(Data1);
                              Data ="";
                              verify((int)ch);
                              Serial.println("IM out");
                              digitalWrite(resetPin, LOW);
                              break;
                          }
                         }
              }         
    
}
bool verify(int a){
    
   int count1 = (a-48) *1000;
    while(1){
       Serial.println(count1);
       if (myGsm.available()>0){
            char ch1 = myGsm.read();
             if(ch1 =='+'){
                return false;
              }
//                    Serial.write(mySerial.read());
       }
     digitalWrite(led2,HIGH);
     digitalWrite(led3,HIGH);
    delay(count1);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    delay(count1);
    }
  }
