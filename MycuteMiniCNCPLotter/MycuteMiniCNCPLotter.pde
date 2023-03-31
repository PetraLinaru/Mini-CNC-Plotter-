import controlP5.*;

ControlP5 cp5; //controlp5 obj
PFont font1,font2,font3;
import processing.serial.*;
Serial port;
String val;

void setup()
{
  printArray(Serial.list()); //print all available ports
  port=new Serial(this,"COM3",9600);
  
  size(800,600); //window size
  cp5 = new ControlP5(this);
  font1=createFont("OCR A Extended",20);
  font2=createFont("OCR A Extended",30);
  font3=createFont("OCR A Extended",15);
  
  //X
  cp5.addButton("Draw_Line_on_X")
  .setPosition(40,80)
  .setSize(230,80)
  .setFont(font1);
  
  //Y
  cp5.addButton("Draw_Line_on_Y")
  .setPosition(40,190)
  .setSize(230,80)
  .setFont(font1);
  
  //BRESENHAM
  cp5.addButton("Draw_Bresenham_Line")
  .setPosition(40,300)
  .setSize(230,80)
  .setFont(font1);
  
  //free move
  cp5.addButton("UP")
  .setPosition(250,410)
  .setSize(70,70)
  .setFont(font1);
   cp5.addButton("DOWN")
  .setPosition(250,500)
  .setSize(70,70)
  .setFont(font1);
   cp5.addButton("LEFT")
  .setPosition(160,455)
  .setSize(70,70)
  .setFont(font1);
   cp5.addButton("RIGHT")
  .setPosition(340,455)
  .setSize(70,70)
  .setFont(font1);
  
  cp5.addButton("PenDown")
  .setPosition(40,410)
  .setSize(60,60)
  .setFont(font3);
  
  cp5.addButton("PenUp")
  .setPosition(40,510)
  .setSize(60,60)
  .setFont(font3);
  
  
  
  

  //TRIANGLE
  //cp5.addButton("Draw Triangle")
  //.setPosition(40,380)
  //.setSize(210,80)
  //.setFont(font1);
}

void draw()
{
  background(245,223,223);
  fill(159,81,100);
  textFont(font2);
  text("My cute CNC Mini-Plotter",200,30);
  if ( port.available() > 0)
  { // daca portul e available
    val = port.readStringUntil('\n');
    while(val==null)
    {
      val="";
    }
  }
   
    print(val);
    
   
  
}

void Draw_Line_on_X()
{
  cp5.addTextfield("XTo")
  .setPosition(380,80)
  .setSize(60,80)
  .setFont(font3);
  cp5.addButton("GO_X")
  .setPosition(470,80)
  .setSize(80,80)
  .setFont(font3);
}

void Draw_Line_on_Y()
{
  cp5.addTextfield("YTo")
  .setPosition(380,190)
  .setSize(60,80)
  .setFont(font3);
  cp5.addButton("GO_Y")
  .setPosition(470,190)
  .setSize(80,80)
  .setFont(font3);
}

void Draw_Bresenham_Line()
{

  cp5.addTextfield("Xf")
  .setPosition(470,300)
  .setSize(60,80)
  .setFont(font3);
   cp5.addTextfield("Yf")
  .setPosition(560,300)
  .setSize(60,80)
  .setFont(font3);
  cp5.addButton("GO")
  .setPosition(650,300)
  .setSize(80,80)
  .setFont(font3);
}



void GO_X() //press the button and it sends data about x positions where to draw the line
{
 
  String X2=cp5.get(Textfield.class,"XTo").getText();
 
  port.write("G X"+X2+" Y"+0+"\n");
}
void GO_Y() //press the button and it sends data about y positions where to draw the line
{
  Float Y2=Float.parseFloat(cp5.get(Textfield.class,"YTo").getText());
  port.write("G X"+0+" Y"+Y2+"\n");
}
void GO()
{
  Float x=Float.parseFloat(cp5.get(Textfield.class,"Xf").getText());
  Float y=Float.parseFloat(cp5.get(Textfield.class,"Yf").getText());
  port.write("G X"+x+" Y"+y+" \n");  
}


void PenDown()
{
  port.write("0 \n");
}
void PenUp()
{
  port.write("1 \n");
}
void UP()
{
  port.write("U \n");
}
void DOWN()
{
  port.write("D \n");
}
void LEFT()
{
  port.write("L \n");
}
void RIGHT()
{
  port.write("R \n");
}

void serialEvent(Serial p)
{
  String s = p.readStringUntil('\n');
  println(s.trim());
  
}
