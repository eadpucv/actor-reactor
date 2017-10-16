import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class signo extends PApplet {

/*

 Proyecci\u00f3n Artefactos y Creaturas
 Octubre 2017
 Taller de Espacios Expositivos e[ad] \u00b7 PUCV
 
 */


PVector[] pt; 
int sides;         
float radius;
Sign signA, signB;



PFont font, greekFont;

int black, white, red, gold;
int[] strokeColor = new int[3];


public void setup() {
  
  //size(1280, 720);
  sides = 9;
  radius = width / 5.0f;   // sign radius
  
  noCursor();
  font = loadFont("Avenir.vlw");  
  greekFont = loadFont("Hellenic.vlw");
  
  // basic 9 positions in the circle
  pt = new PVector[sides];

  for (int i = 0; i < sides; i++) {
    float inc = TWO_PI / PApplet.parseFloat(sides);

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

  signA = new Sign(width * 0.333f, height * 0.5f);
  signB = new Sign(width * 0.666f, height * 0.5f);

  imageMode(CENTER);
  rectMode(CENTER);
} 

public void draw() {
  background(0);

  blendMode(ADD);
  signA.render();
  signB.render();
  blendMode(NORMAL);
  
  /* debug fadeout
  float h1 = map(signA.fade, 0, 255, 0, height);
  float h2 = map(signB.fade, 0, 255, 0, height);
  fill(255);
  rect(10, height/2, 10, h1);
  rect(20, height/2, 10, h2);
  */ 
  
  /* debug lifespan
  fill(255);
  textFont(font);
  text(signA.tic, 20, height - 24);
  */
}
class Line {
  Point p1, p2, c1, c2;
  boolean curve;
  int c;
  float sw, t, tinc;

  Line(int a, int b) {

    p1 = new Point(pt[a].x, pt[a].y);
    p2 = new Point(pt[b].x, pt[b].y);

    t = random(TWO_PI);
    tinc = random(-0.0002f, 0.0002f);

    c = strokeColor[round(random(2))];
    sw = random(.5f, 3);

    if (random(1) > .5f) { // curve-straight probability
      curve = true;
    } else {
      curve = false;
    }

    if (curve) {
      float d = dist(p1.x, p1.y, p2.x, p2.y);
      float amp = map(d, 0, radius * 2, 1, 0);
      c1 = new Point(p1.x * amp + random(-d / 4, d / 4), p1.y * amp + random(-d / 4, d / 4));
      c2 = new Point(p2.x * amp + random(-d / 4, d / 4), p2.y * amp + random(-d / 4, d / 4));
    }
  }

  public void render(PGraphics bitmap, float a) {
    
    bitmap.beginDraw();
    bitmap.pushMatrix();
    bitmap.translate(bitmap.width/2, bitmap.height/2);
    bitmap.rotate(t);
    bitmap.blendMode(BLEND);
    bitmap.stroke(c, a);
    bitmap.strokeWeight(sw);
    bitmap.noFill();
    if (curve) {

      bitmap.bezier(p1.x, p1.y, c1.x, c1.y, c2.x, c2.y, p2.x, p2.y);
      c1.move();
      c2.move();
    } else {
      bitmap.line(p1.x, p1.y, p2.x, p2.y);
      p1.move();
      p2.move();
    }
    bitmap.blendMode(NORMAL);
    bitmap.popMatrix();
    bitmap.endDraw();
    t += tinc;
  }
}
class Point {
  int seed;
  float amp, zoom;
  float x, y, t, s; // x,y, angle and step
  
  Point(float x, float y) {
    this.x = x;
    this.y = y;
    t = random(TWO_PI);
    s = random(.2f, 1);
    
    seed = round(random(100000));
    amp = random(1, 3);
    zoom = random(3000, 10000);
  }

  public void move() {
    
    noiseSeed(seed);
    float dif = .5f - noise((float)millis()/zoom) * amp;
    t += dif;
    
    x += cos(t)*s;
    y += sin(t)*s;
  }
}
class Sign {
  PGraphics bitmap;
  int randomWord;
  float x, y;
  int numLines;
  Line[] lin;
  int start, end;
  int lifespan, tic;
  float existence;
  float alpha, maxalpha;
  float fade;

  Sign (float x, float y) {

    this.x = x;
    this.y = y;
    reset();
    maxalpha = 50;
  }

  public void reset() {
    lifespan = round(random(70, 250));
    existence = lifespan * 1.5f;
    tic = 0;
    fade = 255;
    numLines = round(random(3, 6));
    lin = new Line[numLines];
    int count = 0;
    int lang = round(random(-.44f, 3.44f));
    randomWord = round(random(spanish.length - 1));
    bitmap = createGraphics(PApplet.parseInt(radius * 3), PApplet.parseInt(radius * 3));
    bitmap.beginDraw();
    bitmap.background(0);
    bitmap.fill(255);
    bitmap.textFont(font);

    switch(lang) {
    case 0: /* spanish */
      bitmap.text(spanish[randomWord], radius * .4f, radius * .4f);
      break;
    case 1: /* english */
      bitmap.text(english[randomWord], radius * 1.8f, radius * .4f);
      break;
    case 2: /* german */
      bitmap.text(german[randomWord], radius * .4f, radius * 1.8f);
      break;
    case 3: /* greek */
      bitmap.textFont(greekFont);
      bitmap.text(greek[randomWord], radius * 1.8f, radius * 1.8f);
      break;
    }
    bitmap.endDraw();

    while (count < numLines) {
      start = round(random(sides - 1));
      end = round(random(sides - 1));
      if (start != end) {
        lin[count] = new Line(start, end);
        count++;
      }
    }
  }

  public void updateGraphics() {
    bitmap.beginDraw();
    alpha = sin((float)tic*PI/(float)lifespan) * maxalpha;
    if (tic < lifespan) {
      for (int i = 0; i < lin.length; i++) {
        lin[i].render(bitmap, alpha);
      }
    } 
    bitmap.endDraw();
  }


  public void render() {
    updateGraphics();
    fade = sin((float)tic*PI/existence) * 255;
    tic ++;
    tint(255, fade);
    image(bitmap, x, y);

    if (tic >= existence) {
      this.reset();
    }
  }
}

String[] spanish = {"acci\u00f3n", "aire", "alba", "am\u00e9rica", "amor", "\u00e1rbol", "\u00e1rbol", "armon\u00eda", "ascendente", "aut\u00f3ctono", "ave", "belleza", "beso", "camino", "caracol", "cielo", "confianza", "conyo", "crea", "creaci\u00f3n", "cr\u00f3nica", "cualquiera", "dedicaci\u00f3n", "dise\u00f1o", "ef\u00edmero", "el acto", "el camino", "el desconocido", "el gui\u00f3n", "el oc\u00e9ano", "el oficio", "el rito", "el signo", "empat\u00eda", "esfuerzo", "espacio", "esperanza", "estrellas", "expresi\u00f3n", "felicidad", "flecha", "flor", "fuego", "gato", "grito", "hombres libres", "idea", "ilusi\u00f3n", "inventar", "justicia", "justo", "la extensi\u00f3n", "la fiesta", "la heredad", "la muerte", "la piedad", "la torre", "la tradici\u00f3n", "lejos", "libertad", "libre", "lluvia", "los nueve", "luz ", "mar", "medidas", "memoria", "mirada", "narval", "nervio", "ni\u00f1o", "no", "nobleza", "nube", "nuevo", "ocaso", "paciencia", "palacio", "palma", "p\u00e1nico", "paz", "pensamiento", "perseverancia", "piano", "piedra", "pintura", "poes\u00eda", "red", "renacer", "respeto", "salto", "serenidad", "sobra", "sol", "subjetivo", "sue\u00f1o", "sue\u00f1os", "suerte", "teatro", "torre", "traves\u00eda", "universo", "vida", "viento", "visi\u00f3n", "zoper\u00fatano"};
String[] english = {"action", "air", "sunrise", "America", "love", "tree", "tree", "harmony", "upward", "indigenous", "bird", "beauty", "kiss", "path", "snail", "heaven", "trust", "with me", "creates", "creation", "chronicle", "anyone", "dedication", "design", "ephemeral", "the act", "the road", "the unknown", "the script", "the ocean", "The office", "the rite", "The sign", "empathy", "effort", "space", "hope", "Stars", "expression", "happiness", "arrow", "flower", "fire", "cat", "scream", "Free men", "idea", "delusion", "invent", "justice", "fair", "the extension", "Party", "The inheritance", "death", "The piety", "tower", "the tradition", "far", "freedom", "free", "rain", "The nine", "light", "sea", "measurements", "memory", "look", "narwhal", "nerve", "boy", "do not", "nobility", "cloud", "new", "sunset", "patience", "palace", "palm", "panic", "peace", "thought", "perseverance", "piano", "stone", "painting", "poetry", "net", "be reborn", "respect", "jump", "serenity", "surplus", "Sun", "subjective", "dream", "dreams", "luck", "theater", "tower", "crossing", "universe", "life", "wind", "view", "fool"};
String[] german = {"Aktion", "Luft", "Dawn", "Amerika", "liebe", "Baum", "Baum", "Harmonie", "nach oben", "autochthonen", "Vogel", "Sch\u00f6nheit", "Kuss", "Path", "Schnecke", "Himmel", "Vertrauen", "mit I", "schafft", "Schaffung", "Chronische", "jeder", "Widmung", "Design", "kurzlebig", "Der Akt", "die Art und Weise", "unbekannt", "das Skript", "das Meer", "das B\u00fcro", "der Ritus", "Zeichen", "Empathie", "Anstrengung", "Space", "Hoffnung", "Sterne", "Ausdruck", "Gl\u00fcck", "Pfeil", "Blume", "Feuer", "Katze", "Schrei", "Freemen", "Idee", "Hoffnung", "erfinden", "Gerechtigkeit", "Recht", "Extension", "die Partei", "Vererbung", "Tod", "Piety", "Turm", "Tradition", "weit", "Freiheit", "kostenlos", "regen", "Nine", "Licht", "Meer", "Ma\u00dfnahmen", "Memory", "aussehen", "Narwal", "Nerv", "Kind", "keine", "Adel", "Wolke", "neu", "Twilight", "Geduld", "Palace", "Palme", "Panik", "Frieden", "Denken", "Ausdauer", "Piano", "Stein", "Malerei", "Poesie", "Netzwerk", "wiedergeboren", "Respekt", "springen", "Gelassenheit", "\u00dcberschuss", "Sonne", "subjektiv", "Traum", "Tr\u00e4ume", "Gl\u00fcck", "Theater", "Tower", "Kreuzung", "Universum", "Leben", "Wind", "Ansicht", "Dummkopf"};
String[] greek = {"\u03b4\u03c1\u03ac\u03c3\u03b7", "\u03b1\u03ad\u03c1\u03b1\u03c2", "\u0391\u03c5\u03b3\u03ae", "\u0391\u03bc\u03b5\u03c1\u03b9\u03ba\u03ae", "\u03b1\u03b3\u03ac\u03c0\u03b7", "\u03b4\u03ad\u03bd\u03c4\u03c1\u03bf", "\u03b4\u03ad\u03bd\u03c4\u03c1\u03bf", "\u03b1\u03c1\u03bc\u03bf\u03bd\u03af\u03b1", "\u03b5\u03c0\u03ac\u03bd\u03c9", "\u03b1\u03c5\u03c4\u03cc\u03c7\u03b8\u03c9\u03bd", "\u03c0\u03bf\u03c5\u03bb\u03af", "\u03bf\u03bc\u03bf\u03c1\u03c6\u03b9\u03ac", "\u03c6\u03b9\u03bb\u03af", "\u0394\u03b9\u03b1\u03b4\u03c1\u03bf\u03bc\u03ae", "\u03c3\u03b1\u03bb\u03b9\u03b3\u03ba\u03ac\u03c1\u03b9", "\u03bf\u03c5\u03c1\u03b1\u03bd\u03cc\u03c2", "\u03b5\u03bc\u03c0\u03b9\u03c3\u03c4\u03bf\u03c3\u03cd\u03bd\u03b7", "\u03bc\u03b5 \u0399", "\u03b4\u03b7\u03bc\u03b9\u03bf\u03c5\u03c1\u03b3\u03b5\u03af", "\u03b4\u03b7\u03bc\u03b9\u03bf\u03c5\u03c1\u03b3\u03af\u03b1", "\u03a7\u03c1\u03cc\u03bd\u03b9\u03b1", "\u03ba\u03ac\u03b8\u03b5", "\u03b1\u03c6\u03b9\u03ad\u03c1\u03c9\u03c3\u03b7", "\u03c3\u03c7\u03ad\u03b4\u03b9\u03bf", "\u03b5\u03c6\u03ae\u03bc\u03b5\u03c1\u03bf\u03c2", "\u0397 \u03c0\u03c1\u03ac\u03be\u03b7", "\u03bf \u03c4\u03c1\u03cc\u03c0\u03bf\u03c2", "\u03ac\u03b3\u03bd\u03c9\u03c3\u03c4\u03bf\u03c2", "\u03c4\u03bf \u03c3\u03b5\u03bd\u03ac\u03c1\u03b9\u03bf", "\u03bf \u03c9\u03ba\u03b5\u03b1\u03bd\u03cc\u03c2", "\u03c4\u03bf \u03b3\u03c1\u03b1\u03c6\u03b5\u03af\u03bf", "\u03b7 \u03b9\u03b5\u03c1\u03bf\u03c4\u03b5\u03bb\u03b5\u03c3\u03c4\u03af\u03b1", "\u03c3\u03ae\u03bc\u03b1", "\u03b5\u03bd\u03c3\u03c5\u03bd\u03b1\u03af\u03c3\u03b8\u03b7\u03c3\u03b7", "\u03c0\u03c1\u03bf\u03c3\u03c0\u03ac\u03b8\u03b5\u03b9\u03b1", "\u0394\u03b9\u03ac\u03c3\u03c4\u03b7\u03bc\u03b1", "\u03b5\u03bb\u03c0\u03af\u03b4\u03b1", "\u03b1\u03c3\u03c4\u03ad\u03c1\u03b9\u03b1", "\u03ad\u03ba\u03c6\u03c1\u03b1\u03c3\u03b7", "\u03b5\u03c5\u03c4\u03c5\u03c7\u03af\u03b1", "\u03b2\u03ad\u03bb\u03bf\u03c2", "\u03bb\u03bf\u03c5\u03bb\u03bf\u03cd\u03b4\u03b9", "\u03c6\u03c9\u03c4\u03b9\u03ac", "\u03b3\u03ac\u03c4\u03b1", "\u03ba\u03c1\u03b1\u03c5\u03b3\u03ae", "\u0395\u03bb\u03b5\u03cd\u03b8\u03b5\u03c1\u03bf\u03b9", "\u03b9\u03b4\u03ad\u03b1", "\u03b5\u03bb\u03c0\u03af\u03b4\u03b1", "\u03b5\u03c6\u03b5\u03c5\u03c1\u03af\u03c3\u03ba\u03c9", "\u03b4\u03b9\u03ba\u03b1\u03b9\u03bf\u03c3\u03cd\u03bd\u03b7", "\u03b4\u03b5\u03be\u03b9\u03ac", "\u0395\u03c0\u03ad\u03ba\u03c4\u03b1\u03c3\u03b7", "\u03c4\u03bf \u03ba\u03cc\u03bc\u03bc\u03b1", "\u039a\u03bb\u03b7\u03c1\u03bf\u03bd\u03bf\u03bc\u03b9\u03ac", "\u03b8\u03ac\u03bd\u03b1\u03c4\u03bf\u03c2", "\u0395\u03c5\u03c3\u03ad\u03b2\u03b5\u03b9\u03b1", "\u03c0\u03cd\u03c1\u03b3\u03bf\u03c2", "\u03c0\u03b1\u03c1\u03ac\u03b4\u03bf\u03c3\u03b7", "\u03bc\u03b1\u03ba\u03c1\u03b9\u03ac", "\u03b5\u03bb\u03b5\u03c5\u03b8\u03b5\u03c1\u03af\u03b1", "\u03b4\u03c9\u03c1\u03b5\u03ac\u03bd", "\u03b2\u03c1\u03bf\u03c7\u03ae", "\u0395\u03bd\u03bd\u03ad\u03b1", "\u03c6\u03c9\u03c2", "\u03b8\u03ac\u03bb\u03b1\u03c3\u03c3\u03b1", "\u039c\u03ad\u03c4\u03c1\u03b1", "\u039c\u03bd\u03ae\u03bc\u03b7", "\u03bc\u03b1\u03c4\u03b9\u03ac", "\u03bc\u03bf\u03bd\u03cc\u03ba\u03b5\u03c1\u03c9\u03c2", "\u03bd\u03b5\u03cd\u03c1\u03bf", "\u03c0\u03b1\u03b9\u03b4\u03af", "\u03cc\u03c7\u03b9", "\u03b1\u03c1\u03c7\u03bf\u03bd\u03c4\u03b9\u03ac", "\u03c3\u03cd\u03bd\u03bd\u03b5\u03c6\u03bf", "\u03bd\u03ad\u03bf\u03c2", "\u03bb\u03c5\u03ba\u03cc\u03c6\u03c9\u03c2", "\u03c5\u03c0\u03bf\u03bc\u03bf\u03bd\u03ae", "\u03a0\u03b1\u03bb\u03ac\u03c4\u03b9", "\u03c0\u03b1\u03bb\u03ac\u03bc\u03b7", "\u03c0\u03b1\u03bd\u03b9\u03ba\u03cc\u03c2", "\u03b5\u03b9\u03c1\u03ae\u03bd\u03b7", "\u03c3\u03ba\u03ad\u03c8\u03b7", "\u03b5\u03c0\u03b9\u03bc\u03bf\u03bd\u03ae", "\u039b\u03af\u03c1\u03b1", "\u03c0\u03ad\u03c4\u03c1\u03b1", "\u03b6\u03c9\u03b3\u03c1\u03b1\u03c6\u03b9\u03ba\u03ae", "\u03c0\u03bf\u03af\u03b7\u03c3\u03b7", "\u03b4\u03af\u03ba\u03c4\u03c5\u03bf", "\u03bd\u03b1 \u03be\u03b1\u03bd\u03b1\u03b3\u03b5\u03bd\u03bd\u03b7\u03b8\u03b5\u03af", "\u03c3\u03b5\u03b2\u03b1\u03c3\u03bc\u03cc\u03c2", "\u03ac\u03bb\u03bc\u03b1", "\u03b3\u03b1\u03bb\u03ae\u03bd\u03b7", "\u03c0\u03bb\u03b5\u03cc\u03bd\u03b1\u03c3\u03bc\u03b1", "\u03ae\u03bb\u03b9\u03bf\u03c2", "\u03c5\u03c0\u03bf\u03ba\u03b5\u03b9\u03bc\u03b5\u03bd\u03b9\u03ba\u03cc\u03c2", "\u03cc\u03bd\u03b5\u03b9\u03c1\u03bf", "\u03cc\u03bd\u03b5\u03b9\u03c1\u03b1", "\u03c4\u03cd\u03c7\u03b7", "\u03b8\u03ad\u03b1\u03c4\u03c1\u03bf", "\u03a0\u03cd\u03c1\u03b3\u03bf\u03c2", "\u03b4\u03b9\u03ac\u03b2\u03b1\u03c3\u03b7", "\u03c3\u03cd\u03bc\u03c0\u03b1\u03bd", "\u03b6\u03c9\u03ae", "\u03ac\u03bd\u03b5\u03bc\u03bf\u03c2", "\u03b8\u03ad\u03b1", "\u03b1\u03bd\u03cc\u03b7\u03c4\u03bf\u03c2"};
  public void settings() {  fullScreen(); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "signo" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
