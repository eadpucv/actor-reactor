/*

 cartas de la phalène ~ documenta 14 ~ Atenas
 
 amereida
 
 */


PVector[] pt; 
int sides;         
float radius;
Sign[] signo;



PFont font;
int fs;

color black, white, red, gold;
color[] strokeColor = new color[3];


void setup() {
  //fullScreen();
  size(640, 480);
  sides = 9;
  radius = width / 10;   // sign radius
  fs = width / 48;      // font size

  font = createFont("Helvetica", fs);
  textFont(font, fs);


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

  signo = new Sign[8];

  signo[0] = new Sign(0, width*1/5, height*1/3);
  signo[1] = new Sign(1, width*2/5, height*1/3);
  signo[2] = new Sign(2, width*3/5, height*1/3);
  signo[3] = new Sign(3, width*4/5, height*1/3);
  signo[4] = new Sign(4, width*1/5, height*2/3);
  signo[5] = new Sign(5, width*2/5, height*2/3);
  signo[6] = new Sign(6, width*3/5, height*2/3);
  signo[7] = new Sign(7, width*4/5, height*2/3);

  
}


void draw() {
background(0);
  for (int i = 0; i < signo.length; i++) {
    signo[i].render();
  }
}