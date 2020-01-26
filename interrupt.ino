//ticTocInterrupt
//attachInterrupt()
//digitalPinToInterrupt(pinInt)
//Mega pins 2 3 18 19 20 21
//attachInterrupt(digitalPinToInterrupt(pinInt), ISR, mode)
//ISR (interrupt service routine) function called does not pass or return variables  pass data using volatile global variables
//mode = LOW ; CHANGE; RISING; FALLING (a few boards allow HIGH)
//detachInterrupt(digitalPinToINterrupt(pinInt))
// iterrupts(); turns on interrupts
// nointerrupts(); turns off interrupts 
// iterrupts still register the trigger but do not interrupt until turned on in priority order

//set interupt pin
int trigPin = 20;
volatile bool trigON = true;
volatile unsigned long int trigVal = 0;
//ISR (interrupt service routine) 
void flagTrig(){
  if (trigON){
    trigON = false;
    digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
  } else {
    trigON = true;
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  }
  trigVal += 1;
}
//used tictoc timer as base for interrupt testing
unsigned long int tic[10];
unsigned long int toc[10] = {100, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000};

//once
void setup() {
  String ioOUT;
  pinMode(LED_BUILTIN, OUTPUT);
  flagTrig();//make sure set to off
  trigPin = 2;
  pinMode(trigPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(trigPin), *flagTrig, LOW);

  Serial.begin(115200);
  while(!Serial); //wait for serial to start
  
  ioOUT = F("\nSERIAL START\n");
  ioOUT += F("LOOP DELAY TEST set at ");
  ioOUT += (String)toc[0];
  Serial.println(ioOUT);
  
  //initialize all timers machine time
  for(int x=0; x <= 9; x++){
    tic[x] = millis();
  }
}

//repeat
void loop() {
  String ioOUT;
  tic[0]= millis(); //master loop timer set every loop to check processing delays

//Test for timer trigger and execute timer actions
  for( int x = 1; x <= 9; x++){
    
    //Test for timer duration
    if(millis() - tic[x] >= toc[x]){ //do something for all triggered timers
      tic[x] = millis();//reset timer
      ioOUT = F("\nTimer#");
      ioOUT += (String)x;
      ioOUT += "=";
      ioOUT += toc[x];
      ioOUT += F(" ; triggered at (");
      ioOUT += tic[x];
      ioOUT += ")";
      Serial.print(ioOUT);

      //do stuff if specific timer triggered
      switch(x){
         case 1:
          ioOUT = F("ONE");
          break;
        case 2:
          ioOUT = F("TWO");
          break;
          case 3:
          ioOUT = F("THREE");
        break;
        case 4:
          ioOUT = F("FOUR");
          if(trigON){
            ioOUT = "Interrupt trigger is high and = ";
            ioOUT += (String)trigVal;
          }
          break;
        case 5:
          ioOUT = F("FIVE");
          break;
        case 6:
          ioOUT = F("SIX");
          break;
        case 7:
          ioOUT = F("SEVEN");
          break;
        case 8:
          ioOUT = F("EIGHT");
          break;
        case 9:
          ioOUT = F("NINE");
          break;
        default:
          ioOUT = F("DEFAULT");//should not trigger DEFAULT
          break; 
      }
      Serial.println(ioOUT);
    }
  }
  //overall loop delay excessive
  if(millis() - tic[0] >= toc[0]){
    ioOUT = F("Loop timer delay excessive =");
    ioOUT += (String)(millis() - toc[0]);
    ioOUT += F(" Loop delay greater than ");
    ioOUT += (String)toc[0];
    ioOUT += F(" milliseconds\n");
    Serial.println(ioOUT);
  }
}
