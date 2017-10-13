/*

 cartas de la phalène ~ documenta 14 ~ Atenas
 
 amereida
 
 */

PGraphics overlay;
PVector[] pt;      // coordinates
int sides;         
float radius;
Signo signo;
boolean looping, fading, bang;
String debugText;
int randomText;
PFont font;
int fs;

color black, white, red, gold;
color[] strokeColor = new color[3];

int fadingTime, blackTime;
int timeA, timeB;

boolean tilt;


void setup() {
  //fullScreen();
  size(640, 480);
  sides = 9;
  radius = width / 6;   // sign radius
  fs = width / 28;      // font size

  overlay = createGraphics(width, height);

  looping = false;
  fading = false;
  bang = false;

  fadingTime = 89000;
  blackTime = 9900;


  font = createFont("Helvetica", fs);
  textFont(font, fs);
  randomText = round(random(spanish.length - 1));

  pt = new PVector[sides];
  for (int i = 0; i < sides; i++) {
    float inc = TWO_PI / float(sides);

    // 9 points (pt) distributed in circle (circular identity)
    float xpos = cos(i * inc) * radius;
    float ypos = sin(i * inc) * radius;

    pt[i] = new PVector();
    pt[i].x = xpos;
    pt[i].y = ypos;

    black = color(0, 0, 0);
    white = color(255, 255, 255);
    red = color(255, 68, 0);
    gold = color(209, 194, 143);

    strokeColor[0] = white;
    strokeColor[1] = red;
    strokeColor[2] = gold;
  }

  signo = new Signo(width/2, height/2);

  debugText = "";
  background(0);
  timeA = millis();
  tilt = true;
}

void keyPressed() {
  tilt = !tilt;
  println(tilt);
}

void draw() {
  background(0);
  signo.render();
  drawText();
  if (tilt) {
    fadeOut();
  } else {
    fadeIn();
  }

  image(overlay, 0, 0);
}

void fadeOut() {
  overlay.beginDraw();
  overlay.blendMode(DARKEST);
  overlay.fill(0, 1);
  overlay.noStroke();
  overlay.rect(0, 0, width, height);
  overlay.endDraw();
}

void fadeIn() {
  overlay.beginDraw();
  overlay.blendMode(LIGHTEST);
  overlay.fill(255, 1);
  overlay.noStroke();
  overlay.rect(0, 0, width, height);
  overlay.endDraw();
}

/*
void draw() {
 
 if (millis() - timeA > blackTime) {
 bang = true;
 timeA = millis();
 }
 
 if (bang) {
 signo = new Signo(width / 2, height / 2);
 strokeWeight(1);
 stroke(255);
 signo.render();
 drawText();
 bang = false;
 fading = true;
 timeB = millis();
 }
 
 blendMode(BLEND);
 signo.render();
 blendMode(NORMAL);
 
 if (fading) {
 blendMode(DARKEST);
 
 fill(0, 1);
 noStroke();
 rect(0, 0, width, height);
 if (millis() - timeB > fadingTime) {
 fading = false;
 timeA = millis();
 }
 blendMode(NORMAL);
 }
 } 
 */

void drawText() {
  fill(255);
  noStroke();
  textAlign(CENTER);
  pushMatrix();
  translate(width / 2, height / 2);

  // english - top (180º)
  pushMatrix();
  translate(0, -height / 2 + fs);
  rotate(PI);
  text(english[randomText], 0, 0);
  popMatrix();

  // spanish - bottom (0º)
  pushMatrix();
  translate(0, height / 2 - fs);
  text(spanish[randomText], 0, 0);
  popMatrix();

  // greek - left (90º)
  pushMatrix();
  translate(-width / 2 + fs, 0);
  rotate(HALF_PI);
  text(greek[randomText], 0, 0);
  popMatrix();

  // german - right (-90º)
  pushMatrix();
  translate(width / 2 - fs, 0);
  rotate(-HALF_PI);
  text(german[randomText], 0, 0);
  popMatrix();
  popMatrix();
}