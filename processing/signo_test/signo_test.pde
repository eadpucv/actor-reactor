/*

 Proyección Artefactos y Creaturas
 Octubre 2017
 Taller de Espacios Expositivos e[ad] · PUCV
 
 */


PVector[] pt; 
int sides;         
float radius;
Sign signA, signB;



PFont font, greekFont;

color black, white, red, gold;
color[] strokeColor = new color[3];


void setup() {
  fullScreen();
  // size(1280, 720);
  sides = 9;
  radius = width / 5.0;   // sign radius
  
  noCursor();
  font = loadFont("Avenir.vlw");  
  greekFont = loadFont("Hellenic.vlw");
  
  // basic 9 positions in the circle
  pt = new PVector[sides];

  for (int i = 0; i < sides; i++) {
    float inc = TWO_PI / float(sides);

    // 9 points (pt) distributed in circle (circular identity)
    float xpos = cos(i * inc) * radius;
    float ypos = sin(i * inc) * radius;

    pt[i] = new PVector();
    pt[i].x = xpos;
    pt[i].y = ypos;
  }

  black = color(0, 0, 0);
  white = color(255, 255, 255);
  red = color(255, 68, 0);
  gold = color(142, 127, 80);

  strokeColor[0] = white;
  strokeColor[1] = red;
  strokeColor[2] = gold;

  signA = new Sign(width * 0.333, height * 0.5);
  signB = new Sign(width * 0.666, height * 0.5);

  imageMode(CENTER);
  rectMode(CENTER);
  // textMode(CENTER);
} 

void draw() {
  background(0);

  blendMode(ADD);
  signA.render();
  signB.render();
  blendMode(NORMAL);
}