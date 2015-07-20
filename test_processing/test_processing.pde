import processing.serial.*;

static int max_height = 500;
static int max_width = 1000;

int transmission_count = 0;

int xpos = 0;
int ypos = 0;

int rad = 10;

Serial port;

int index_value = 0;


int x_next = 0;
int y_next = 0;

IntList dataset;

ArrayList < String > data;
float[] floating_data;

PrintWriter output;

int calculate_x(int x)
{
  int xposition = height;
  
  
  if (x == 25) return xposition;
  else if (x < 25) return xposition + x;
  else return xposition - x;
}


void setup()
{
  size(max_width, max_height);
  //stroke(150,150, 150);
  //String arduinoPort = Serial.list()[0];
  
  String arduinoPort = "COM4";
  println(Serial.list());

  port = new Serial(this, arduinoPort, 9600);
  xpos = height;
  ypos = width / 2;
  
  x_next = 0;
  //y_next = ypos - 25;
  
  strokeWeight(10);
  
  output = createWriter("bot_sensor_value.txt");
  data = new ArrayList < String >();
  dataset = new IntList();
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

void plot(String inp)
{
  dataset.append(int(inp));
  index_value++;
  if (index_value == 1){
    y_next = ypos - 25;
    line(xpos, ypos, calculate_x(dataset.get(0)), y_next);
  }
  
  else {
    int xnext = calculate_x(dataset.get(index_value));
    int xprev = calculate_x(dataset.get(index_value-1));
    int ynext = y_next - (25 * index_value);
    int yprev = y_next - (25 * ( index_value - 1));
    
    line(xprev, yprev, xnext, ynext);
  }
}
  
  
  
  

//
void processString(String in)
{
  if (in.contains("Begin")) transmission_count++; 
  
  else if (!in.contains("stop") && transmission_count >= 5 && !in.contains("Begin")) { println(in);  plot(in); } //data.add(in);
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


