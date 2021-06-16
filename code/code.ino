#include <SevSeg.h>
#include <EEPROM.h>

SevSeg sevseg;

int light;
int light2;
int light3;
int light4;
int light5;
int light6;

int last_light = 0;
int last_light2 = 0;
int last_light3 = 0;
int last_light4 = 0;
int last_light5 = 0;
int last_light6 = 0;

int delta_light = 0;
int delta_light2 = 0;
int delta_light3 = 0;
int delta_light4 = 0;
int delta_light5 = 0;
int delta_light6 = 0;

int delay_piece = 500;

int total = 0;

int compteur = 0;
int id_ecriture = 10;

void get_analog_value(){
  light = analogRead(A0);
  light2 = analogRead(A1);
  light3 = analogRead(A2);
  light4 = analogRead(A3);
  light5 = analogRead(A4);
  light6 = analogRead(A5);
}

void update_last_analog_value(){
  last_light = light;
  last_light2 = light2;
  last_light3 = light3;
  last_light4 = light4;
  last_light5 = light5;
  last_light6 = light6;
}

void update_delta_light(){
  delta_light = last_light - light;
  delta_light2 = last_light2 - light2;
  delta_light3 = last_light3 - light3;
  delta_light4 = last_light4 - light4;
  delta_light5 = last_light5 - light5;
  delta_light6 = last_light6 - light6;
}

void write_on_memory(int id,int data){
  EEPROM.put(id,data);
}

int get_from_memory(int id){
  int a;
  EEPROM.get(id,a);
  return a;
}

boolean get_money = false;

void setup(void) {
  byte numDigits = 4;
  byte digitPins[] = {13, 12, 11, 10};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};

  bool resistorsOnSegments = true; 
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_CATHODE; 
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(100);

  
  Serial.begin(9600);
}

void loop(void) {
  if(get_money == false){
    get_analog_value();
    update_last_analog_value();
    for(int i = 0; i < 50; i++){
      Serial.println("loading");
    }
    get_money = true;
  }else{
    update_delta_light();
    update_last_analog_value();
    get_analog_value();
    if(delta_light <= -2.5){
      Serial.println("10 centimes");
      total+=10;
      delay(delay_piece);
    }else if(delta_light2 <= -3){
      Serial.println("5 centimes");
      delay(delay_piece);
      total+=5;
    }else if(delta_light3 <= -4){
      Serial.println("20 centimes");
      total+=20;
      delay(delay_piece);
    }else if(delta_light4 <= -2.25){
      Serial.println("1 euro");
      total+=100;
      delay(delay_piece);
    }else if(delta_light5 <= -2.5){
      Serial.println("50 centimes");
      total+=50;
      delay(delay_piece);
    }else if(delta_light6 <= -3){
      Serial.println("2 euros");
      total+=200;
      delay(delay_piece);
    }
    sevseg.setNumber(total,2);
    sevseg.refreshDisplay();
    compteur += 1;
    if(compteur == 1000){
      ecrire_memoire(id_ecriture,total);
      Serial.print("Données sauvegardées :");
      Serial.print(total);
      Serial.println(" centimes");
      compteur = 0;
    }
    delay(5);
  }
}
