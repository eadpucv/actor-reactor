/*

 cartas de la phalène ~ documenta 14 ~ Atenas
 
 amereida
 
 */


PVector[] pt;      // coordinates
int sides;         
float radius;
Signo signo;

String debugText;
int randomText;
PFont font;
int fs;

color black, white, red, gold;
color[] strokeColor = new color[3];


boolean fading = true;
int fadingTime = 1000;
float fadeFactor;
float alpha = 255.0;
int flag;
float t; // rotation angle

void setup() {
  //fullScreen();
  size(640, 480);
  sides = 9;
  radius = width / 6;   // sign radius
  fs = width / 48;      // font size


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

  fadeFactor = 255.0 / float(fadingTime);
  println(fadeFactor);
  background(0);
}


void draw() {
  
  signo.render();
  drawText();

  alpha -= fadeFactor;
  if (alpha < 0) {
    fading = false;
    alpha = 255;
    flag = frameCount;
    signo = new Signo(width/2, height/2);
    background(0);
  }
  
  blendMode(DARKEST);
  fill(0, 2);
  noStroke();
  rect(0, 0, width, height);
  blendMode(NORMAL);
  t+= 0.002;
}


void drawText() {
  fill(255, alpha);
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