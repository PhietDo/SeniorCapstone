import processing.serial.*;
Serial myPort; // The serial port
//set the height and width of the screen window
int width = 1400; 
int height = 700;
int origin = 0;
//initialize variables to obtain from the Arduino
float [] strainVal = new float[6];
float xVal = 0;
float yVal = 0;
float zVal = 0;
String [] dataInfo;
//positions for rendering the line
int xPos = 0;
int xPosTwo = 0;
float rainbow = 0;
//float startSpot = 21.8;
int firstRow = 1;
int secondRow = 9;
int thirdRow = 17;

/*
  Initial setup for the COM port and the window mode
*/
void setup() {
  myPort = new Serial(this, "COM2", 115200); //change COM to whatever master device you are using
  // A serialEvent() is generated when a newline character is received :
  myPort.bufferUntil('\n');
  size(1400,700);
  background(#182825); // set inital background:
}

void draw() {
  //calls a set functions to render the axes and other preset components
  drawTicks(firstRow); 
  drawTicks(secondRow);
  drawTicks(thirdRow);
  drawAxis();
  drawBoxes();

  
  //Maps the strain values and accelerometer values to a certain axes  
  float startXLeft = map(xPos, 0, 698, 0, 698);
  float startXRight = map(xPosTwo, 2, 702, 702, 1400);
  
  float strainOne = map(strainVal[0], 0, 400, 87, 10);
  float strainTwo = map(strainVal[1], 0, 400, 261, 200);
  float strainThree = map(strainVal[2], 0, 400, 435, 375);
  
  float strainFour = map(strainVal[3], 0, 400, 87, 10);
  float strainFive = map(strainVal[4], 0, 400, 261, 200);
  float strainSix = map(strainVal[5], 0, 400, 435, 375);
  
  float xValue = map(xVal, 0, 3, 609, 550); 
  //float yValue = map(yVal, 0, 3, 438, 375);
  float zValue = map(zVal, 0, 3, 609, 550);
  //**************************************
  //Draws all the lines located on the left side of the window
  //LEFT SIDE
  strokeWeight(2);
  stroke(255, rainbow, 0);
  line(startXLeft, strainOne, startXLeft, strainOne);
  
  strokeWeight(2);
  stroke(0, 255, rainbow);
  line(startXLeft, strainTwo, startXLeft, strainTwo);
  
  strokeWeight(2);
  stroke(rainbow, 0, 255);
  line(startXLeft, strainThree, startXLeft, strainThree);
  
  stroke(255, 255-rainbow, 80);
  line(startXLeft, xValue, startXLeft, xValue);
  
  //**************************************
  //Draws all the lines located on the right side of the window
  //RIGHT SIDE
  strokeWeight(2);
  stroke(255-rainbow, 255, 0);
  line(startXRight, strainFour, startXRight, strainFour);
  
  strokeWeight(2);
  stroke(0, 255-rainbow, 255);
  line(startXRight, strainFive, startXRight, strainFive);
  
  strokeWeight(2);
  stroke(255, 0, 255-rainbow);
  line(startXRight, strainSix, startXRight, strainSix);
  
  stroke(255-rainbow, 180, 255-rainbow);
  line(startXRight, zValue, startXRight, zValue);
  
   rainbow = rainbowCheck(rainbow);
  
  //if the line has hit the edge of the graph, then reset the position
  if (xPosTwo >= width/2) {
    rainbow = 0;
    xPos = 0;
    xPosTwo = 0;
    strainVal[1] = 0;
    //background(0);
    background(#182825); // set inital background:
    
    //increase to the next position of in the graph
  } else {
    xPos++;
    xPosTwo++;
    
  }
  
}

//function to draw axis on the graph in both the X and Y direction.
void drawAxis(){
  strokeWeight(2); //stroke wider
  stroke(#A2A79E); //stroke color
  //START X, START Y, END X, END Y
  line(width/2, origin, width/2, height);// vertical middle line
  int n = 1;
  for(int x=0; x<3; x++){
    line(origin, (height/4)*n, width, (height/4)*n);// horizontal first middle line
    n++;
  }
  stroke(#426B69); //stroke color
  strokeWeight(0.5); //stroke wider
  //drawing the section horizontal lines
  int m = 1;
  for(int x=0; x<4; x++){
    line(0, (height/8)*m, (width/2)-2, (height/8)*m); // horizontaltopleft
    line((width/2)+2, (height/8)*m, width, (height/8)*m); // horizontaltopright
    m += 2;
  }
}

//function to draw boxes that will hold the values and display the data in time to the user
void drawBoxes(){
  stroke(#A2A79E);
  fill(#88292F);
  rect(398, 154, 300, 20);
  rect(1070, 154, 300, 20);
  rect(398, 328, 300, 20);
  rect(1070, 328, 300, 20);
  rect(398, 503, 300, 20);
  rect(1070, 503, 300, 20);
  rect(398, 680, 300, 20);
  rect(1070, 680, 300, 20);
  
  noStroke();
  fill(255, 255, 255);
  textSize(16);

  text("Strain1: ", 478, 170); // tells you where write the number
  text(strainVal[0], 578, 170); // tells you where write the number
  
  text("Strain2: ", 478, 345); // tells you where write the number
  text(strainVal[1], 578, 345); // tells you where write the number
  
  text("Strain3: ", 478, 520); // tells you where write the number
  text(strainVal[2], 578, 520); // tells you where write the number
  
  text("X G-Force: ", 478, 695); // tells you where write the number
  text(xVal, 578, 695); // tells you where write the number
  //*******************************
  
  text("Strain4: ", 1148, 170); // tells you where write the number
  text(strainVal[3], 1248, 170); // tells you where write the number
  
  text("Strain5: ", 1148, 345); // tells you where write the number
  text(strainVal[4], 1248, 345); // tells you where write the number
  
  text("Strain6: ", 1148, 520); // tells you where write the number
  text(strainVal[5], 1248, 520); // tells you where write the number
  
  text("Z G-Force: ", 1148, 695); // tells you where write the number
  text(zVal, 1248, 695); // tells you where write the number
}

//resets the color of the line
static float rainbowCheck(float rainbow){
  if(rainbow > 255){
   rainbow -=0.5;
  }
  else{
   rainbow+=0.5; 
  }
  return rainbow;
}

//draws the tick marks on the graph with increments SPECIFICALLY for strain values
void drawTicks(int row){
  float graphOne = 21.8;
  int spot = row;
  int startingStrVal = 300;
  float interval = 0;
  int accelLoc = 543;
  int startLeftOne = 0;
  int startLeftTwo = 20;
  int startRightOne = 700;
  int startRightTwo = 720;
  stroke(#62827c);
  textSize(8);
  fill(255);
  
  for (int x = 1; x <8; x++) {
    interval = graphOne*spot;
    
    line(startLeftOne, interval, startLeftTwo, interval); // horizontal1 ticks
    text(startingStrVal, 5, interval-5);
    startingStrVal -= 100;
    spot++;
  }
  
  graphOne = 21.8;
  startingStrVal = 300;
  spot = row;
  for (int x = 1; x <8; x++) {
    interval = graphOne*spot;
    
    line(startRightOne, interval, startRightTwo, interval); // horizontal1 ticks
    text(startingStrVal, 705, interval-5);
    startingStrVal -= 100;
    spot++;
  }
  
  
  drawAccelTicks(accelLoc);
}

//Subfunction that draws the tick marks on the graph with increments SPECIFICALLY for acceleration values
void drawAccelTicks(int startPos){
  int startPosLeft = startPos;
  int startPosRight = startPos;
  int accelVal = 3;
   for(int x = 7; x>0; x--){
      
     line(0, startPosLeft, 20, startPosLeft); // horizontal1 ticks
     text(accelVal, 12, startPosLeft-2);
     accelVal--;
     startPosLeft += 22;
   }
   accelVal = 3;
   for(int x = 7; x>0; x--){  
     line(700, startPosRight, 720, startPosRight); // horizontal1 ticks
     text(accelVal, 712, startPosRight-2);
     accelVal--;
     startPosRight += 22;
   }
}

//serial function to grab the value from the Arduino and place it in array of values
void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');
  if (inString != null){
    inString = trim(inString); // trim off whitespaces.
    dataInfo = split(inString, ",");
    //currentVal1 = float(inString); // convert to a number.
    strainVal[0] = float(dataInfo [0]);
    strainVal[1] = float(dataInfo [1]);
    strainVal[2] = float(dataInfo [2]);
    strainVal[3] = float(dataInfo [3]);
    strainVal[4] = float(dataInfo [4]);
    strainVal[5] = float(dataInfo [5]);
    xVal = float(dataInfo [6]);
    //yVal = float(dataInfo [2]);
    zVal = float(dataInfo [7]);
  }
  redraw();
}