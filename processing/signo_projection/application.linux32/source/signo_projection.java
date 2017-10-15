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

public class signo_projection extends PApplet {

/*

 Artefactos y Creaturas
 
 Proyecci\u00f3n de 8 signos en transici\u00f3n
 
 */


PVector[] pt; 
int sides;         
float radius;
Sign[] signo;



PFont font;
int fs;

int black, white, red, gold;
int[] strokeColor = new int[3];


public void setup() {
  
  // size(640, 480);
  sides = 9;
  radius = width / 10;   // sign radius
  fs = width / 48;      // font size

  font = createFont("Helvetica", fs);
  textFont(font, fs);


  pt = new PVector[sides];
  for (int i = 0; i < sides; i++) {
    float inc = TWO_PI / PApplet.parseFloat(sides);

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


public void draw() {
background(0);
  for (int i = 0; i < signo.length; i++) {
    signo[i].render();
  }
}
class Line {
  Point c1, c2;
  float x1, x2, y1, y2, cx1, cy1, cx2, cy2;
  boolean curve;
  int c;
  float sw;

  Line(int a, int b) {

    x1 = pt[a].x;
    y1 = pt[a].y;
    x2 = pt[b].x;
    y2 = pt[b].y;

    c = strokeColor[round(random(2))];
    sw = random(.5f, 2);

    if (random(1) > .5f) { // curve-straight probability
      curve = true;
    } else {
      curve = false;
    }

    if (curve) {
      float d = dist(x1, y1, x2, y2);
      float amp = map(d, 0, radius * 2, 1, 0);

      c1 = new Point(x1 * amp + random(-d / 4, d / 4), y1 * amp + random(-d / 4, d / 4));
      c2 = new Point(x2 * amp + random(-d / 4, d / 4), y2 * amp + random(-d / 4, d / 4));
    }
  }

  public void render(float a) {
    stroke(c, a);
    strokeWeight(sw);
    noFill();
    if (curve) {
      bezier(x1, y1, c1.x, c1.y, c2.x, c2.y, x2, y2);
      c1.move();
      c2.move();
    } else {
      line(x1, y1, x2, y2);
    }
  }
}
class Point {
  float x, y, t, s; // x,y, angle and step
  Point(float x, float y) {
    this.x = x;
    this.y = y;
    t = random(TWO_PI);
    s = random(0, 1);
  }

  public void move() {
    x += cos(t)*s;
    y += sin(t)*s;
  }
}
class Sign {
  int index;
  float x, y, t, tinc;
  int numLines;
  Line[] lin;
  int start, end;
  int lifespan, tic;
  int randomText;
  float alpha;

  Sign (int i, float x, float y) {

    lifespan = round(random(30, 111));
    tic = 0;

    index = i;
    this.x = x;
    this.y = y;

    numLines = round(random(3, 6));
    lin = new Line[numLines];

    t = random(TWO_PI);
    tinc = random(-0.005f, 0.005f);

    int count = 0;
    randomText = round(random(spanish.length - 1));

    while (count < numLines) {

      start = round(random(sides - 1));
      end = round(random(sides - 1));

      if (start != end) {
        lin[count] = new Line(start, end);
        count++;
      }
    }
  }

  public void render() {

    alpha = sin((float)tic*PI/(float)lifespan) * 180;

    if (tic < lifespan) {
      pushMatrix();
      translate(x, y);
      rotate(t);
      for (int i = 0; i < lin.length; i++) {
        lin[i].render(alpha);
      }
      popMatrix();
    }
    
    tic ++;
    t += tinc;

    if (tic > lifespan * 2) {
      signo[index] = new Sign(index, x, y);
      fill(0);
      noStroke();
      ellipse(x, y, 2.1f * radius, 2.1f * radius);
    }
  }
}
String[] spanish = {"acci\u00f3n", "aire", "alba", "am\u00e9rica", "amor", "\u00e1rbol", "\u00e1rbol", "armon\u00eda", "ascendente", "aut\u00f3ctono", "ave", "belleza", "beso", "camino", "caracol", "cielo", "confianza", "conyo", "crea", "creaci\u00f3n", "cr\u00f3nica", "cualquiera", "dedicaci\u00f3n", "dise\u00f1o", "ef\u00edmero", "el acto", "el camino", "el desconocido", "el gui\u00f3n", "el oc\u00e9ano", "el oficio", "el rito", "el signo", "empat\u00eda", "esfuerzo", "espacio", "esperanza", "estrellas", "expresi\u00f3n", "felicidad", "flecha", "flor", "fuego", "gato", "grito", "hombres libres", "idea", "ilusi\u00f3n", "inventar", "justicia", "justo", "la extensi\u00f3n", "la fiesta", "la heredad", "la muerte", "la piedad", "la torre", "la tradici\u00f3n", "lejos", "libertad", "libre", "lluvia", "los nueve", "luz ", "mar", "medidas", "memoria", "mirada", "narval", "nervio", "ni\u00f1o", "no", "nobleza", "nube", "nuevo", "ocaso", "paciencia", "palacio", "palma", "p\u00e1nico", "paz", "pensamiento", "perseverancia", "piano", "piedra", "pintura", "poes\u00eda", "red", "renacer", "respeto", "salto", "serenidad", "sobra", "sol", "subjetivo", "sue\u00f1o", "sue\u00f1os", "suerte", "teatro", "torre", "traves\u00eda", "universo", "vida", "viento", "visi\u00f3n", "zoper\u00fatano"};
String[] english = {"action", "air", "sunrise", "America", "love", "tree", "tree", "harmony", "upward", "indigenous", "bird", "beauty", "kiss", "path", "snail", "heaven", "trust", "with me", "creates", "creation", "chronicle", "anyone", "dedication", "design", "ephemeral", "the act", "the road", "the unknown", "the script", "the ocean", "The office", "the rite", "The sign", "empathy", "effort", "space", "hope", "Stars", "expression", "happiness", "arrow", "flower", "fire", "cat", "scream", "Free men", "idea", "delusion", "invent", "justice", "fair", "the extension", "Party", "The inheritance", "death", "The piety", "tower", "the tradition", "far", "freedom", "free", "rain", "The nine", "light", "sea", "measurements", "memory", "look", "narwhal", "nerve", "boy", "do not", "nobility", "cloud", "new", "sunset", "patience", "palace", "palm", "panic", "peace", "thought", "perseverance", "piano", "stone", "painting", "poetry", "net", "be reborn", "respect", "jump", "serenity", "surplus", "Sun", "subjective", "dream", "dreams", "luck", "theater", "tower", "crossing", "universe", "life", "wind", "view", "fool"};
String[] german = {"Aktion", "Luft", "Dawn", "Amerika", "liebe", "Baum", "Baum", "Harmonie", "nach oben", "autochthonen", "Vogel", "Sch\u00f6nheit", "Kuss", "Path", "Schnecke", "Himmel", "Vertrauen", "mit I", "schafft", "Schaffung", "Chronische", "jeder", "Widmung", "Design", "kurzlebig", "Der Akt", "die Art und Weise", "unbekannt", "das Skript", "das Meer", "das B\u00fcro", "der Ritus", "Zeichen", "Empathie", "Anstrengung", "Space", "Hoffnung", "Sterne", "Ausdruck", "Gl\u00fcck", "Pfeil", "Blume", "Feuer", "Katze", "Schrei", "Freemen", "Idee", "Hoffnung", "erfinden", "Gerechtigkeit", "Recht", "Extension", "die Partei", "Vererbung", "Tod", "Piety", "Turm", "Tradition", "weit", "Freiheit", "kostenlos", "regen", "Nine", "Licht", "Meer", "Ma\u00dfnahmen", "Memory", "aussehen", "Narwal", "Nerv", "Kind", "keine", "Adel", "Wolke", "neu", "Twilight", "Geduld", "Palace", "Palme", "Panik", "Frieden", "Denken", "Ausdauer", "Piano", "Stein", "Malerei", "Poesie", "Netzwerk", "wiedergeboren", "Respekt", "springen", "Gelassenheit", "\u00dcberschuss", "Sonne", "subjektiv", "Traum", "Tr\u00e4ume", "Gl\u00fcck", "Theater", "Tower", "Kreuzung", "Universum", "Leben", "Wind", "Ansicht", "Dummkopf"};
String[] greek = {"\u03b4\u03c1\u03ac\u03c3\u03b7", "\u03b1\u03ad\u03c1\u03b1\u03c2", "\u0391\u03c5\u03b3\u03ae", "\u0391\u03bc\u03b5\u03c1\u03b9\u03ba\u03ae", "\u03b1\u03b3\u03ac\u03c0\u03b7", "\u03b4\u03ad\u03bd\u03c4\u03c1\u03bf", "\u03b4\u03ad\u03bd\u03c4\u03c1\u03bf", "\u03b1\u03c1\u03bc\u03bf\u03bd\u03af\u03b1", "\u03b5\u03c0\u03ac\u03bd\u03c9", "\u03b1\u03c5\u03c4\u03cc\u03c7\u03b8\u03c9\u03bd", "\u03c0\u03bf\u03c5\u03bb\u03af", "\u03bf\u03bc\u03bf\u03c1\u03c6\u03b9\u03ac", "\u03c6\u03b9\u03bb\u03af", "\u0394\u03b9\u03b1\u03b4\u03c1\u03bf\u03bc\u03ae", "\u03c3\u03b1\u03bb\u03b9\u03b3\u03ba\u03ac\u03c1\u03b9", "\u03bf\u03c5\u03c1\u03b1\u03bd\u03cc\u03c2", "\u03b5\u03bc\u03c0\u03b9\u03c3\u03c4\u03bf\u03c3\u03cd\u03bd\u03b7", "\u03bc\u03b5 \u0399", "\u03b4\u03b7\u03bc\u03b9\u03bf\u03c5\u03c1\u03b3\u03b5\u03af", "\u03b4\u03b7\u03bc\u03b9\u03bf\u03c5\u03c1\u03b3\u03af\u03b1", "\u03a7\u03c1\u03cc\u03bd\u03b9\u03b1", "\u03ba\u03ac\u03b8\u03b5", "\u03b1\u03c6\u03b9\u03ad\u03c1\u03c9\u03c3\u03b7", "\u03c3\u03c7\u03ad\u03b4\u03b9\u03bf", "\u03b5\u03c6\u03ae\u03bc\u03b5\u03c1\u03bf\u03c2", "\u0397 \u03c0\u03c1\u03ac\u03be\u03b7", "\u03bf \u03c4\u03c1\u03cc\u03c0\u03bf\u03c2", "\u03ac\u03b3\u03bd\u03c9\u03c3\u03c4\u03bf\u03c2", "\u03c4\u03bf \u03c3\u03b5\u03bd\u03ac\u03c1\u03b9\u03bf", "\u03bf \u03c9\u03ba\u03b5\u03b1\u03bd\u03cc\u03c2", "\u03c4\u03bf \u03b3\u03c1\u03b1\u03c6\u03b5\u03af\u03bf", "\u03b7 \u03b9\u03b5\u03c1\u03bf\u03c4\u03b5\u03bb\u03b5\u03c3\u03c4\u03af\u03b1", "\u03c3\u03ae\u03bc\u03b1", "\u03b5\u03bd\u03c3\u03c5\u03bd\u03b1\u03af\u03c3\u03b8\u03b7\u03c3\u03b7", "\u03c0\u03c1\u03bf\u03c3\u03c0\u03ac\u03b8\u03b5\u03b9\u03b1", "\u0394\u03b9\u03ac\u03c3\u03c4\u03b7\u03bc\u03b1", "\u03b5\u03bb\u03c0\u03af\u03b4\u03b1", "\u03b1\u03c3\u03c4\u03ad\u03c1\u03b9\u03b1", "\u03ad\u03ba\u03c6\u03c1\u03b1\u03c3\u03b7", "\u03b5\u03c5\u03c4\u03c5\u03c7\u03af\u03b1", "\u03b2\u03ad\u03bb\u03bf\u03c2", "\u03bb\u03bf\u03c5\u03bb\u03bf\u03cd\u03b4\u03b9", "\u03c6\u03c9\u03c4\u03b9\u03ac", "\u03b3\u03ac\u03c4\u03b1", "\u03ba\u03c1\u03b1\u03c5\u03b3\u03ae", "\u0395\u03bb\u03b5\u03cd\u03b8\u03b5\u03c1\u03bf\u03b9", "\u03b9\u03b4\u03ad\u03b1", "\u03b5\u03bb\u03c0\u03af\u03b4\u03b1", "\u03b5\u03c6\u03b5\u03c5\u03c1\u03af\u03c3\u03ba\u03c9", "\u03b4\u03b9\u03ba\u03b1\u03b9\u03bf\u03c3\u03cd\u03bd\u03b7", "\u03b4\u03b5\u03be\u03b9\u03ac", "\u0395\u03c0\u03ad\u03ba\u03c4\u03b1\u03c3\u03b7", "\u03c4\u03bf \u03ba\u03cc\u03bc\u03bc\u03b1", "\u039a\u03bb\u03b7\u03c1\u03bf\u03bd\u03bf\u03bc\u03b9\u03ac", "\u03b8\u03ac\u03bd\u03b1\u03c4\u03bf\u03c2", "\u0395\u03c5\u03c3\u03ad\u03b2\u03b5\u03b9\u03b1", "\u03c0\u03cd\u03c1\u03b3\u03bf\u03c2", "\u03c0\u03b1\u03c1\u03ac\u03b4\u03bf\u03c3\u03b7", "\u03bc\u03b1\u03ba\u03c1\u03b9\u03ac", "\u03b5\u03bb\u03b5\u03c5\u03b8\u03b5\u03c1\u03af\u03b1", "\u03b4\u03c9\u03c1\u03b5\u03ac\u03bd", "\u03b2\u03c1\u03bf\u03c7\u03ae", "\u0395\u03bd\u03bd\u03ad\u03b1", "\u03c6\u03c9\u03c2", "\u03b8\u03ac\u03bb\u03b1\u03c3\u03c3\u03b1", "\u039c\u03ad\u03c4\u03c1\u03b1", "\u039c\u03bd\u03ae\u03bc\u03b7", "\u03bc\u03b1\u03c4\u03b9\u03ac", "\u03bc\u03bf\u03bd\u03cc\u03ba\u03b5\u03c1\u03c9\u03c2", "\u03bd\u03b5\u03cd\u03c1\u03bf", "\u03c0\u03b1\u03b9\u03b4\u03af", "\u03cc\u03c7\u03b9", "\u03b1\u03c1\u03c7\u03bf\u03bd\u03c4\u03b9\u03ac", "\u03c3\u03cd\u03bd\u03bd\u03b5\u03c6\u03bf", "\u03bd\u03ad\u03bf\u03c2", "\u03bb\u03c5\u03ba\u03cc\u03c6\u03c9\u03c2", "\u03c5\u03c0\u03bf\u03bc\u03bf\u03bd\u03ae", "\u03a0\u03b1\u03bb\u03ac\u03c4\u03b9", "\u03c0\u03b1\u03bb\u03ac\u03bc\u03b7", "\u03c0\u03b1\u03bd\u03b9\u03ba\u03cc\u03c2", "\u03b5\u03b9\u03c1\u03ae\u03bd\u03b7", "\u03c3\u03ba\u03ad\u03c8\u03b7", "\u03b5\u03c0\u03b9\u03bc\u03bf\u03bd\u03ae", "\u039b\u03af\u03c1\u03b1", "\u03c0\u03ad\u03c4\u03c1\u03b1", "\u03b6\u03c9\u03b3\u03c1\u03b1\u03c6\u03b9\u03ba\u03ae", "\u03c0\u03bf\u03af\u03b7\u03c3\u03b7", "\u03b4\u03af\u03ba\u03c4\u03c5\u03bf", "\u03bd\u03b1 \u03be\u03b1\u03bd\u03b1\u03b3\u03b5\u03bd\u03bd\u03b7\u03b8\u03b5\u03af", "\u03c3\u03b5\u03b2\u03b1\u03c3\u03bc\u03cc\u03c2", "\u03ac\u03bb\u03bc\u03b1", "\u03b3\u03b1\u03bb\u03ae\u03bd\u03b7", "\u03c0\u03bb\u03b5\u03cc\u03bd\u03b1\u03c3\u03bc\u03b1", "\u03ae\u03bb\u03b9\u03bf\u03c2", "\u03c5\u03c0\u03bf\u03ba\u03b5\u03b9\u03bc\u03b5\u03bd\u03b9\u03ba\u03cc\u03c2", "\u03cc\u03bd\u03b5\u03b9\u03c1\u03bf", "\u03cc\u03bd\u03b5\u03b9\u03c1\u03b1", "\u03c4\u03cd\u03c7\u03b7", "\u03b8\u03ad\u03b1\u03c4\u03c1\u03bf", "\u03a0\u03cd\u03c1\u03b3\u03bf\u03c2", "\u03b4\u03b9\u03ac\u03b2\u03b1\u03c3\u03b7", "\u03c3\u03cd\u03bc\u03c0\u03b1\u03bd", "\u03b6\u03c9\u03ae", "\u03ac\u03bd\u03b5\u03bc\u03bf\u03c2", "\u03b8\u03ad\u03b1", "\u03b1\u03bd\u03cc\u03b7\u03c4\u03bf\u03c2"};
  public void settings() {  fullScreen(); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "signo_projection" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
