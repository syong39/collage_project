int Pin1 = D1;
int Pin2 = D2;
int Pin3 = D3;
int Pin4 = D4; //스텝모터의 핀 설정

int pole1[] ={0,0,0,0, 0,1,1,1, 0};
int pole2[] ={0,0,0,1, 1,1,0,0, 0};
int pole3[] ={0,1,1,1, 0,0,0,0, 0};
int pole4[] ={1,1,0,0, 0,0,0,1, 0}; //각 스텝 설정
int poleStep = 0; // 0~7까지의 스텝 동작. 8은 정지상태
//8 step values

int val = 0; //패턴설정을 위한 변수값 지정
int  dirStatus = 3;//3= stop, 각 상황들을 지정하기 위한 변수

String buttonTitle1[] ={"<<<", ">>>","패턴1","패턴2"}; 
String buttonTitle2[] ={"<<<", ">>>","패턴1","패턴2"}; //버튼
String argId[] ={"ccw", "cw","pat1","pat2"}; //웹페이지 링크를 구성하는 부분

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "SSHS"
#define STAPSK  "19-70002892" //핫스팟 아이디와 비밀번호
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

void handleRoot() { //웹페이지를 만드는 부분

   //Motor Control
 String HTML ="<!DOCTYPE html>\
  <html>\
  <head>\
  \t\n<title>우리아이모빌</title>\
  \t\n<meta charset=\"utf-8\">\
  \t\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  \n<style>\
 \nhtml,body{\t\nwidth:100%\;\nheight:100%\;\nmargin:0}\n*{box-sizing:border-box}\n.colorAll{\n\tbackground-color:#90ee90}\n.colorBtn{\n\tbackground-color:#add8e6}\n.angleButtdon,a{\n\tfont-size:30px\;\nborder:1px solid #ccc\;\ndisplay:table-caption\;\npadding:7px 10px\;\ntext-decoration:none\;\ncursor:pointer\;\npadding:5px 6px 7px 10px}a{\n\tdisplay:block}\n.btn{\n\tmargin:5px\;\nborder:none\;\ndisplay:inline-block\;\nvertical-align:middle\;\ntext-align:center\;\nwhite-space:nowrap}\n";
   
  HTML +="</style>\n\n</head>\n\n<body>\n<h1>&#129392;우리아이 모빌&#129392;</h1>\n";

   if(dirStatus ==2){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">모빌이 시계방향으로 작동합니다</span></h2>\n";    
   }else if(dirStatus ==1){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">모빌이 반시계방향으로 작동합니다</span></h2>\n";      
   }else if(dirStatus ==4){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">패턴1</span></h2>\n";    
   }else if(dirStatus ==5){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">패턴2</span></h2>\n";    
   }else{
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">멈춰있습니다</span></h2>\n";    
   }
   
      if(dirStatus ==1){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[0];
        HTML += "=off\">";
        HTML +=buttonTitle1[0]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[0];
        HTML += "=on\">";       
        HTML +=buttonTitle2[0];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";  

      if(dirStatus ==2){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[1];
        HTML += "=off\">";
        HTML +=buttonTitle1[1]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[1];
        HTML += "=on\">";       
        HTML +=buttonTitle2[1];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";    
      
     if(dirStatus ==4){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[2];
        HTML += "=off\">";
        HTML +=buttonTitle1[2]; 
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[2];
        HTML += "=on\">";       
        HTML +=buttonTitle2[2];
      }   
     HTML +="</a>\t\n\t</div>\n\n";   

      if(dirStatus ==5){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[3];
        HTML += "=off\">";
        HTML +=buttonTitle1[3]; 
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[3];
        HTML += "=on\">";       
        HTML +=buttonTitle2[3];
      }   
     HTML +="</a>\t\n\t</div>\n\n";   

  HTML +="\t\n</body>\n</html>\n";
  server.send(200, "text/html", HTML);  

}

void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);

}

void setup(void) {

 pinMode(Pin1, OUTPUT);
 pinMode(Pin2, OUTPUT);
 pinMode(Pin3, OUTPUT);
 pinMode(Pin4, OUTPUT);

 
  Serial.begin(115200);//initialize the serial monitor

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.print("IP address: http://");
  Serial.println(WiFi.localIP());


  server.on("/", handleRoot);
  server.on("/motor", HTTP_GET, motorControl); 
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  
//각 상황일때 어떤 동작을 하는지.
if(dirStatus ==1){ 
   poleStep++;
    driveStepper(poleStep);    
 }else if(dirStatus ==2){ 
   poleStep--; 
    driveStepper(poleStep);
 }
 
 else if(dirStatus ==4){ //패턴1
  if(val<4000){
    poleStep++;
    val++;
  } else if (val<8000){
    poleStep--;
    val++;
  }else if (val=8000){
      val=0;
      val++;
      poleStep++;
    }
      driveStepper(poleStep);
 }

  else if(dirStatus ==5){ //패턴2
  if(val<1000){
    poleStep++;
    val++;
  } else if (val<3000){
    poleStep--;
    val++;
  }else if (val<6000){
      poleStep++;
      val++;
    }else if (val<10000){
      poleStep--;
      val++;

    }else if (val=10000){
      poleStep++;
      val=0;
    }
      driveStepper(poleStep);
 }
 
 else{
  driveStepper(8);
  val=0;
 }
 
 if(poleStep>7){ 
   poleStep=0; 
 } 
 if(poleStep<0){ 
   poleStep=7; 
 } 

        delay(1); //딜레이를 줘서 모터의 속도를 조정할 수 있음
}


void motorControl() { //각 링크에 접속했을 시 상황 부여.

    if(server.arg(argId[0]) == "on")
    {
      dirStatus = 1;// CCW 
               
    }else if(server.arg(argId[0]) == "off"){
      dirStatus = 3;  // motor OFF 
          
    }else if(server.arg(argId[1]) == "on"){
      dirStatus = 2;  // CW  
          
    }else if(server.arg(argId[1]) == "off"){
      dirStatus = 3;  // motor OFF 
    }
    
    else if(server.arg(argId[2])=="on"){
      dirStatus = 4; //패턴
      }
    else if(server.arg(argId[2])=="off"){
      dirStatus = 3; //패턴
      }
  
    else if(server.arg(argId[3])=="on"){
      dirStatus = 5; //패턴
      }
    else if(server.arg(argId[3])=="off"){
      dirStatus = 3; //패턴
      }
  
  handleRoot(); //상황부여 이후 그다음 웹 소스를 구현하라.
}


void driveStepper(int c)
{ //스텝을 움직이기 위한 함수
 
     digitalWrite(Pin1, pole1[c]);  
     digitalWrite(Pin2, pole2[c]); 
     digitalWrite(Pin3, pole3[c]); 
     digitalWrite(Pin4, pole4[c]);   

}
