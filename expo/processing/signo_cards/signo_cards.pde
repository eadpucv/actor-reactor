/*

 cartas de la phalène ~ documenta 14 ~ Atenas
 
 amereida
 
 */


PVector[] pt;      // coordinates
int sides;         
float radius;
Signo signo;
boolean looping;
String debugText;
int randomText;
PFont font;
int fs;
float rot;



void setup() {
  size(375, 667);
  frameRate(18);
  sides = 9;
  looping = true;
  rot = 0;
  radius = width / 3;
  fs = width / 18;
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
  }

  signo = new Signo(width/2, height/3);

  debugText = "";
  background(255);
}



void draw() {
  noFill();
  if (looping) {

    signo = new Signo(width / 2, height / 3);

    strokeWeight(5);
    stroke(255, 50);
    signo.render();
    strokeWeight(1);
    stroke(255);
    signo.render();
    fill(0, 20);
    noStroke();
    rect(0, 0, width, height);
    rot += .002;
  } else {
    background(255);
    strokeWeight(1.5);
    stroke(0);
    signo.render();
    drawText();
  }
}

void drawText() {
  fill(0);
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

void mouseReleased() {
  looping = !looping;
  randomText = round(random(spanish.length - 1));
  /*
	if (!looping) {
   		saveCanvas('myCanvas', 'jpg');
   	}*/
}