/* 

BY TSAFACK Jordane - GOHEP Maurelle  06/03/2017
Mini projet 2016-2017
*/
#define  digitalPinToInterrupt(p) ((p)==2 ? 0 : ((p)==3 ? 1 : ((p) >=18&&(p)<=21 ? 23-(p) :  -1)))


#include"TimerOne.h"
#include<LiquidCrystal.h>

// déclarations des variables globales 
const byte ledRouge=37,ledVerte=36 ,photoR =A0, bp1=18;
byte colonneLigne0=0 ,colonneLigne1=0 ;
int loff,lum;

int codeCarDemod=0 ;
byte compteur =0 ;
int tempsHIgh=15,tempsInterrupt=10 ;
long timer2=0,temp1=80,temp0=160 ;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//fin déclaration des variables globales 

void setup() {
  //initialisation de la carte definition des entrée /sortie,interruption,Timer1. NB: pour l'entrée analogique (A0) pas besoins de faire car par défaut est en entrée 

  
    pinMode(ledRouge,OUTPUT);

    digitalWrite(ledRouge,LOW);
    
    Serial.begin(115200) ;
    
    
    
    lcd.begin(16,2);
    lcd.clear() ;
    Serial.println("Ready!!") ;
    
    loff=analogRead(photoR) ;

    pinMode(ledVerte,OUTPUT);
    digitalWrite(ledVerte,HIGH) ;
    
    Timer1.initialize() ;
    Timer1.setPeriod(tempsInterrupt*1000) ;
    Timer1.attachInterrupt(demoduler) ;
    Timer1.start() ;
    timer2=millis() ;
    attachInterrupt(digitalPinToInterrupt(bp1),effacerLcd,RISING) ;


}

void loop() {
  // put your main code here, to run repeatedly:




     while(Serial.available()) {

      
        digitalWrite(ledVerte,LOW) ;
        char c=Serial.read() ;
        lcd.setCursor(colonneLigne0,0) ;
        lcd.print(c) ;
        colonneLigne0=(colonneLigne0+1)%16 ;
        
        Serial.println(c) ;
   
       // début de la communication

        moduler(c) ; 

       digitalWrite(ledRouge,LOW)  ;
       //fin de la communication 
     
      
       digitalWrite(ledVerte,HIGH) ;// on remet la led verte on
   
  }
  
}






void  moduler(char c) {
  int val=c ;
  Serial.print("  Code: :") ;
  Serial.print(val) ;
  int recuperBit=1 ;


  for (int i=7;i>=0;i--){
    recuperBit=1<<i ;
  
      if( recuperBit&val){
               digitalWrite(ledRouge,HIGH) ;
        delay(tempsHIgh) ;
    
         digitalWrite(ledRouge,LOW) ;
         delay(temp1) ;
         digitalWrite(ledRouge,HIGH) ;
         delay(tempsHIgh) ;
        
      }
  
      else{
        digitalWrite(ledRouge,HIGH) ;
        delay(tempsHIgh) ;
        digitalWrite(ledRouge,LOW) ;
        delay(temp0) ;
        digitalWrite(ledRouge,HIGH) ;
        delay(tempsHIgh) ;
  
      }
      
  }
  
  
 }




void demoduler (){

 lum=analogRead(photoR) ;
//detecter si la led rouge brille 
  if((lum-loff)<-50 ){

       
          long timer=millis()-timer2 ;

          if(timer>=temp1-2*tempsHIgh&& timer<temp1+tempsHIgh*2){
              
              Serial.print("  of  timer :") ; 
              Serial.print(timer) ;
              Serial.println(" 1") ;
              codeCarDemod+=(1<<(7-compteur)) ;
              Serial.print("  C :") ;
              Serial.println(codeCarDemod) ;   
              compteur++ ;
             
             }
          
            else if(timer>=temp0-2*tempsHIgh-10&& timer<temp0+tempsHIgh*2){
            
            Serial.print("  of  timer :") ; 
            Serial.print(timer) ;
            Serial.println("  0") ; 
             compteur++ ;  
           
             }
          
              if( compteur==8){

               char c=codeCarDemod ;
               Serial.println(" car recu") ;
               Serial.println(c) ;
               lcd.setCursor(colonneLigne1,1) ;
               lcd.print(c) ;
               colonneLigne1=(colonneLigne1+1)%16 ;
               compteur =codeCarDemod=0 ;
               
               }
          
               timer2=millis();
          


     

  }

}





void effacerLcd(){
  
  
  lcd.clear()  ;
  colonneLigne1=colonneLigne0=0 ;
  
  
  
  
  
  
  
  
  
  }





    
