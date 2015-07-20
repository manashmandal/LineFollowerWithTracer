import processing.serial.*;

static int max_height = 500;
static int max_width = 1000;

String out = "";

int transmission_count = 0;

int xpos = 0;
int ypos = 0;

int sub = 25;

int x_next = 0;
int y_next;

IntList dataset;

int index = 0;


Serial port;

int calculate_x(int x)
{
  if (x == 25) return xpos;
  else if (x < 25) return xpos + x;
  else return xpos - x;
}

void draw() {}

void setup()
{
  
  size(max_width, max_height);
  xpos = height;
  ypos = width / 2;
  strokeWeight(10);

  y_next = ypos - sub;

  String arduinoPort = "COM4";
  port = new Serial(this, arduinoPort, 9600);
  
  dataset = new IntList();
    
}

void plot(String inp)
{
  background(255);
  
 index++;
 int val = parseInt(inp);
 println(val);
 dataset.append(val);
 

   if (index == 1) line(xpos, ypos, calculate_x(dataset.get(index-1)), y_next);


  
  for (index = 1; index < dataset.size(); index++){
   int xnext = calculate_x(dataset.get(index));
   int xprev = calculate_x(dataset.get(index - 1));
   int yprev = ypos - sub * (index);
   int ynext = ypos - (sub * (index + 1));
  
   line(xprev, yprev, xnext, ynext); 
  
 } 
 
 for (int i = 0; i < dataset.size(); i++){
  println("value[ " + i + "] = " + dataset.get(i)); 
 }
 
}

void processString(String in)
{
  if (in.contains("Begin")) transmission_count++;
  else if (transmission_count > 2 && !in.contains("stop") && !in.contains("Begin")) { plot(in);}
  else if (in.contains("stop")) println("fucking stopped");
}


void serialEvent(Serial port)
{
  
  try {
    String inByte = port.readStringUntil('\n');
    if (inByte != null){
      inByte = trim(inByte);
      out = inByte.toString();
      processString(out);
    }
  } catch (Exception e){
    println("Initialization Failed");
  }
}
