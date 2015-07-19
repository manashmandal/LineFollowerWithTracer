import processing.serial.*;

int xpos = 0;
int ypos = 0;

int rad = 10;

Serial port;

ArrayList < String > data;
float[] floating_data;

PrintWriter output;


void setup()
{
  size(300, 300);
  background(0);
  stroke(150,150, 150);
  //String arduinoPort = Serial.list()[0];
  
  String arduinoPort = "COM4";
  println(Serial.list());

  port = new Serial(this, arduinoPort, 9600);
  xpos = width / 2;
  ypos = height / 2;
  
  output = createWriter("bot_sensor_value.txt");
  data = new ArrayList < String >();
  
}

void draw()
{
  //Nothing to do in draw, doing it all in serialEvent
}


void printToFile()
{
 
  floating_data = new float[data.size()];
  
  for (int i = 0; i < data.size(); i++){
    floating_data[i] = float(data.get(i));
  }
  
  for (int i = 0; i < floating_data.length; i++){
    print(floating_data[i]);
    println();
  }
  
}

//
void processString(String in)
{

  if (!in.contains("stop"))  println(in);   //data.add(in);
  else if (in.contains("stop")) println("fucking stopped"); // printToFile();

////  if (!in.contains("stop")) {println(in); output.println(in);}
////  else if (in.contains("stop")) {println("fucking stop"); output.println("fucking stop"); output.flush(); output.close();}
}

void serialEvent(Serial port){
  String inByte = port.readStringUntil('\n');
  //processString(inByte);
  if (inByte != null) inByte = trim(inByte);
  String balsal = "";
  if (inByte != null) {balsal = inByte.toString();
  processString(balsal);
  }
}


