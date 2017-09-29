#include <Wire.h>

int clock = 8;
int reset = 9;

void shift_bit(){
  digitalWrite(clock, HIGH);
  digitalWrite(clock, LOW);
}

void reset_shift(){
  digitalWrite(reset, HIGH);
  shift_bit();
  digitalWrite(reset, LOW);
}

void reset_matrix(){
  int i;
  for(i=0;i<12;i++){
    shift_bit();
  }
  //registo A integrado da esquerda
  Wire.beginTransmission(0x24);
  Wire.write(0x12); // address port A
  Wire.write(0xFF);  // desliga saidas
  Wire.endTransmission();
  
  //registo B integrado da esquerda
  Wire.beginTransmission(0x24);
  Wire.write(0x13); // address port B
  Wire.write(0xFF);  // desliga saidas
  Wire.endTransmission();

  //registo A integrado do meio
  Wire.beginTransmission(0x26);
  Wire.write(0x12); // address port A
  Wire.write(0xFF);  // desliga saidas
  Wire.endTransmission();

  //registo B integrado do meio
  Wire.beginTransmission(0x26);
  Wire.write(0x13); // address port B
  Wire.write(0xFF);  // desliga saidas
  Wire.endTransmission();

  //registo A integrado do direita
  Wire.beginTransmission(0x27);
  Wire.write(0x12); // address port A
  Wire.write(0xFF);  // desliga saidas
  Wire.endTransmission();

  //registo B integrado da direita
  Wire.beginTransmission(0x27);
  Wire.write(0x13); // address port B
  Wire.write(0xFF);  // desliga saidas
  Wire.endTransmission();
}

void setup_cubo(){
  Wire.begin();

  //integrado da esquerda
  Wire.beginTransmission(0x24);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x03); // registos 0 e 1 como saídas e resto como entradas
  Wire.endTransmission();
  
  Wire.beginTransmission(0x24);
  Wire.write(0x01); // IODIRB register
  Wire.write(0xC0); // registos 7 e 6 como saídas e resto como entradas
  Wire.endTransmission();


  //integrado da meio
  Wire.beginTransmission(0x26);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x03); // registos 0 e 1 como saídas e resto como entradas
  Wire.endTransmission();

  Wire.beginTransmission(0x26);
  Wire.write(0x01); // IODIRB register
  Wire.write(0xC0); // registos 7 e 6 como saídas e resto como entradas
  Wire.endTransmission();

  //integrado da direita
  Wire.beginTransmission(0x27);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x03); // registos 0 e 1 como saídas e resto como entradas
  Wire.endTransmission();

  //resistencias de pull_up
  Wire.beginTransmission(0x27);
  Wire.write(0x0C); // address port A
  Wire.write(0x03);  // value to send
  Wire.endTransmission();
  
  Wire.beginTransmission(0x27);
  Wire.write(0x01); // IODIRB register
  Wire.write(0xC0); // registos 7 e 6 como saídas e resto como entradas
  Wire.endTransmission();

  //resistencias de pull_up
  Wire.beginTransmission(0x27);
  Wire.write(0x0D); // address port A
  Wire.write(0xC0);  // value to send

  //Configurar pinos do shift register
  pinMode(clock, OUTPUT); //clock
  pinMode(reset, OUTPUT); //reset

  reset_matrix();
}

void setup() {
  setup_cubo();
}

void loop() {
  byte inA,inB;
  Serial.begin(9600);
  //Serial.print("I received: ");
  //Wire.beginTransmission(0x27);
  //Wire.write(0x13);

  Wire.requestFrom(0x27, 2);

  while(Wire.available())    // slave may send less than requested
  {
    inA = Wire.read();
    inB = Wire.read();
    inA = inA & 0x03;
    inB = inB & 0xC0;
    //botao cima 1
    if(inA == 1){
        Serial.print("Carregou no botao de cima\n");
    }
    //botao baixo 2
    if(inA == 2){
        Serial.print("Carregou no botao de baixo\n");
    }
    
  }
  delay(50);
  //in = ini [0];
  //delay(100);

  // say what you got:
  //Serial.print("\nI received: ");
  //Serial.println(in, DEC);

}
