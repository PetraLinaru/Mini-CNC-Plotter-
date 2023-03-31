
#include <Servo.h>
#include <Stepper.h>
#define xDir 5 // clockwise/ counter-clockwise
#define yDir 6 //clockwise/ counter-clockwise
              //clockwise->1, counter-clockwise->0

//Motoare si configuratie
const int stepsPerRevolution = 48;
const int axisSpeed=60; //60 rpm
Stepper myStepperX(27,2,5);
//Stepper myStepperY(55,3,6);

//Steps
//La 10 impulsuri a 55 steps (550 steps) -> avem 20mm
// ceea ce inseamna ca la 1 mm vom avea 550/20=27.5 steps
//la fel pe x si y;
float stepsPerMilim = 55;
float StepInc=1;

/////////////////////////////////////////////

//Servo
Servo penServo;  
const int penZUp = 180; //Servo sus
const int penZDown= 90; //Servo jos
const int penServoPin = 11; // Pin-ul Z+, care are ca si corespondend D11 pe Arduino UNO
const int enable = 8;  //Activam motoarele pe 0
/////////////////////////////////////////////

struct point
{
  float x;
  float y;
};

struct point currentPoint;

/////////////////////////////////////////////

//Limite pentru desenare
float Xmin = 0;
float Xmax = 39;
float Ymin = 0;
float Ymax = 39;
float Zmin = 0;
float Zmax = 1;

float Xpos = Xmin;
float Ypos = Ymin;
float Zpos = Zmax; 
/////////////////////////////////////////////

// Miscarile pe care le interpretez:
// G1 x y -> miscare pe x si y pe care le primesc de la interfata ( ce introduce utilizatorul )
// U, D, L ,R -> miscare up down left right de la interfata


void setup() {
   Serial.begin( 9600 );
   //Motoarele sunt active pe 0
  pinMode(enable,OUTPUT);
  digitalWrite(enable, LOW);
  //atasam servo-ul
  penServo.attach(penServoPin);
  penServo.write(penZDown);
  delay(200);
  penServo.write(penZUp);
  delay(300);

   //setam viteza cu care desenam
     myStepperX.setSpeed(130);
  //   myStepperY.setSpeed(110);
 //    myStepperX.step(27);
   //  myStepperY.step(55);


  //digitalWrite(5,HIGH);
}


void loop() {

  delay(300);
  char line[ 256 ]; //sa speram ca nu avem nevoie de mai mult de 256 pt o comanda
  char c;
  int lineIndex;
  
  bool lineIsComment, lineSemiColon;

  lineIndex = 0;


  while (true) {
    //asteptam comenzile primite de la serial monitor prin interfata seriala
    while ( Serial.available()>0 ) {
      c = Serial.read();
      if (( c == '\n') || (c == '\r') ) {             // am ajuns la finalul liniei
        if ( lineIndex > 0 ) {                        // chiar am ajuns la finalul liniei
          line[ lineIndex ] = '\0';                   // am parcurs toata comanda si marcam terminarea
          Serial.println(line);
          Serial.print("\n");
          getCommand( line, lineIndex );              // procesam comanda
          lineIndex = 0;                              // resetam index-ul
        } 
      } 
      else {
          if ( c <= ' ' ) {                           // ignoram spatiile albe
          } 
          if ( c >= 'a' && c <= 'z' ) {               // typos
            line[ lineIndex++ ] = c-'a'+'A';
          } 
          else {
            line[ lineIndex++ ] = c;                   //pur si simplu stocam informatia
          }
        }
      }
    }
}

void getCommand( char* line, int nchars ) {
  int currentIndex = 0;
  char buffer[ 64 ];                                 // Hope that 64 is enough for 1 parameter
  struct point newPos;

  newPos.x = 0.0;
  newPos.y = 0.0;
  Serial.println("Aici");



  while( currentIndex < nchars ) {
    switch ( line[ currentIndex++ ] ) {              // Select command, if any
    case '0':
      putPenDown();
      break;
    case '1':
      putPenUp();
      break;  
//    case 'U':
//       if(currentPoint.y <= Ymin)
//      {
//       // nu pot merge in spate mai mult decat imi permite spatiul de desenare 
//      }
//      else
//      {
//        myStepperY.step(-55);
//        currentPoint.y-=1;
//      }
//      break;
//    case 'D':
//      if(currentPoint.y >= Ymax)
//      {
//       // nu pot merge in spate mai mult decat imi permite spatiul de desenare 
//      }
//      else
//      {
//        myStepperY.step(55);
//        currentPoint.y+=1;
//      }
//      break;
    case 'L':
       if(currentPoint.x >= Xmax)
      {
       // nu pot merge in spate mai mult decat imi permite spatiul de desenare 
      }
      else
      {
        myStepperX.step(27);
        currentPoint.x+=1;
      }
      break;
    case 'R':
      if(currentPoint.x <= Xmin)
      {
       //nu pot merge mai in fata decat imi permite spatiul de desenare
      }
      else
      {
        myStepperX.step(-27);
        currentPoint.x-=1;
      }
        break;
    case 'G':
        char* indexX = strchr( line+currentIndex, 'X' );  // Gasim pozitia pt x si y
        
        Serial.println("Indexul lui x:");
        Serial.print(indexX);
        Serial.print("\n");
        
        char* indexY = strchr( line+currentIndex, 'Y' );
        
        Serial.print("Indexul lui y:");
        Serial.print(indexY);
        Serial.print("\n");
        
        if ( indexY <= 0 ) {
          newPos.x = atof( indexX + 1); 
          newPos.y = currentPoint.y;
        } 
        else if ( indexX <= 0 ) {
          newPos.y = atof( indexY + 1);
          newPos.x = currentPoint.x;
        } 
        else {
          newPos.y = atof( indexY + 1);
          *indexY = '\0';
          newPos.x = atof( indexX + 1);
        }
       Serial.print("NewPos:");
       Serial.print(newPos.x);
       Serial.print(newPos.y);
       Serial.print("\n");
       delay(200);
       drawBresenhamLine(newPos.x, newPos.y );
       
       currentPoint.x = newPos.x;                         //dam update la noua pozitie
       currentPoint.y = newPos.y;
       
        break;
      }
    }
 }

int sign(float x)
{
if(x>0)
 return 1;
else if(x<0)
 return -1;
else
 return 0;
}

void putPen()
{
  delay(200);
  penServo.write(penZDown);
  delay(150);
  penServo.write(penZUp);
  delay(200);
}

void putPenUp()
{
  delay(150);
  penServo.write(penZUp);
  delay(200);
}
void putPenDown()
{
  delay(200);
  penServo.write(penZDown);
  delay(150);
}

void drawBresenhamLine(float x1, float y1)
{

  Serial.println("Bresenham");
  if (x1 >= Xmax) { 
    x1 = Xmax; 
  }
  if (x1 <= Xmin) { 
    x1 = Xmin; 
  }
  if (y1 >= Ymax) { 
    y1 = Ymax; 
  }
  if (y1 <= Ymin) { 
    y1 = Ymin; 
  }
  
  //cati pasi avem nevoie sa  
 
  Serial.print("X Y Values:");
  Serial.print(x1);
  Serial.print(y1);
  Serial.print("\n");
  
  float x0 = Xpos*27.5;
  float y0 = Ypos*27.5;
  float over=0;

  //calculam in steps/mm
  x1=x1*27.5;
  y1=y1*27.5;

  //
  float dx =abs(x1-x0);
  float dy =abs(y1-y0);
  int sx = sign(x1-x0); 
  int sy = sign(y1-y0);

  float i;
  long p = 2*dy-dx;
  float temp;
  int swap=0;

  putPen();
   if(dy>dx)
     {
     temp=dx;
     dx=dy;
     dy=temp;
     swap=1;
     }
  p=2*dy-dx;
  for(i=0;i<dx;i+=27.5)
   {
   putPen();
   while(p>=0)
    {
    p=p-2*dx;
    Serial.println("P is ");
    Serial.print(p);
    Serial.println("\n");
    delay(500);
    
    if(swap){
     myStepperX.step(27);
    }
    else
     Serial.println("m-am blocat in prima bucla a, unde i este:");
     Serial.print(i);
     Serial.println("\n");
     delay(500);
//     myStepperY.step(55);
    }
   p=p-2*dy;
   if(swap){
    Serial.println("m-am blocat in a doua bucla, unde i este:");
    Serial.print(i);
    Serial.println("\n");
 //   myStepperY.step(55);
   }
   else
   {
     myStepperX.step(27);
   }
   }
  putPen();
}




  
    
 
