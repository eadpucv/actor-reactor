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
  
  // size(1280, 720);
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
  // textMode(CENTER);
} 

public void draw() {
  background(0);
  blendMode(ADD);
  signA.render();
  signB.render();
  blendMode(NORMAL);
}
class Line {
  Point p1, p2, c1, c2;
  boolean curve;
  int c;
  float sw, t, tinc;

  Line(int a, int b) {

    p1 = new Point(pt[a].x, pt[a].y);
    p2 = new Point(pt[b].x, pt[b].y);

    p1.s = random(.1f, .5f);
    p2.s = random(.1f, .5f);
    
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
    } else {
      float A = 0.8f;
      float B = 1 - A;
      c1 = new Point(p1.x * A + p2.x * B, p1.y * A + p2.y * B);
      c2 = new Point(p1.x * B + p2.x * A, p1.y * B + p2.y * A);
    }
    
    c1.s = random(.5f, 1.5f);
    c2.s = random(.5f, 1.5f);
  }

  public void render(PGraphics bm, float a) {

    bm.rotate(t);
    bm.blendMode(BLEND);
    bm.stroke(c, a);
    bm.strokeWeight(sw);
    bm.noFill();
    bm.bezier(p1.x, p1.y, c1.x, c1.y, c2.x, c2.y, p2.x, p2.y);
    
    p1.move();
    p2.move();
    c1.move();
    c2.move();

    bm.blendMode(NORMAL);
   
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
  boolean init;

  Sign (float x, float y) {
    this.x = x;
    this.y = y;
    reset();
    maxalpha = 50;  // maximum line opacity
    
  }

  public void reset() {
    init = true;
    lifespan = round(random(70, 250));
    existence = lifespan * 1.5f;          // lifespan + blacktime
    tic = 0;
    fade = 255;
    numLines = round(random(3, 6));
    lin = new Line[numLines];
    int count = 0;

    randomWord = round(random(spanish.length - 1));
    bitmap = createGraphics(PApplet.parseInt(radius * 3), PApplet.parseInt(radius * 3));
    while (count < numLines) {
      start = round(random(sides - 1));
      end = round(random(sides - 1));
      if (start != end) {
        lin[count] = new Line(start, end);
        count++;
      }
    }
  }


  public void drawText() {
    
    int lang = PApplet.parseInt(random(5));
    
    bitmap.textFont(font);
    bitmap.fill(white);

    float k = radius * .666f;
    float xpos = random(-2*k, k/2);
    float ypos = random(-k, k);

    switch(lang) {
    case 0: /* spanish */
      bitmap.text(spanish[randomWord], xpos, ypos);
      break;
    case 1: /* english */
      bitmap.text(english[randomWord], xpos, ypos);
      break;
    case 2: /* german */
      bitmap.text(german[randomWord], xpos, ypos);
      break;
    case 3: /* greek */
      bitmap.textFont(greekFont);
      bitmap.text(greek[randomWord], xpos, ypos);
      break;
    case 4: /* french */
      bitmap.text(french[randomWord], xpos, ypos);
      break;
    }
  }

  public void updateGraphics() {
    bitmap.beginDraw();

    bitmap.pushMatrix();
    bitmap.translate(bitmap.width/2, bitmap.height/2);

    if (init) {
      drawText();
      init = false;
    }

    alpha = sin((float)tic*PI/(float)lifespan) * maxalpha;
    if (tic < lifespan) {
      for (int i = 0; i < lin.length; i++) {
        lin[i].render(bitmap, alpha);
      }
    } 
    
    bitmap.popMatrix();
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
String[] spanish = {"am\u00e9rica","\u00e1rbol","libertad","empat\u00eda","palma","viento","ave","pensamiento","perseverancia","sue\u00f1o","grito","salto","sol","narval","cualquiera","sue\u00f1os","gato","armon\u00eda","mar","aut\u00f3ctono","piedra","red","visi\u00f3n","subjetivo","no","flor","belleza","ef\u00edmero","teatro","aire","flecha","confianza","paz","zoper\u00fatano","paciencia","dise\u00f1o","expresi\u00f3n","ascendente","pintura","traves\u00eda","lluvia","estrellas","caracol","nervio","cielo","acci\u00f3n","espacio","beso","nobleza","idea","luz ","ilusi\u00f3n","armon\u00eda","esfuerzo","luz","flor","luz","paz","crea","inventar","libre","sobra","ni\u00f1o","nuevo","amor","sol","renacer","creaci\u00f3n","felicidad","mirada","flor","lejos","libertad","sol","dedicaci\u00f3n","justo","suerte","luz","luz","aire","sue\u00f1o","\u00e1rbol","respeto","vida","universo","cielo","esperanza","nuevo","serenidad","felicidad","mar","p\u00e1nico","nube","poes\u00eda","fuego","vida","justicia","Anglo","esperanza","vida","piano","sol","esfuerzo","familia","lucha","verso","contigo","\u00e1rbol","flora","luna","esperanza","vida","altura","amabilidad","naturaleza","sol","sue\u00f1o","camino","expresi\u00f3n","odisea","ideas","cambio","nuevo","andar","tonto","origen","noche","libertad","expansi\u00f3n","h\u00edbrida","la bandada","asincron\u00eda","elevada","el aro","fragmentar","la extensi\u00f3n","el cuerpo","descalce","ampliaci\u00f3n","los perfiles","impulso","acorazado","el valle","elegancia","segmentada","el pu\u00f1o","v\u00ednculo","sinuoso","la flor","arritmia","dispar","el vuelo","distensi\u00f3n","palpitar","el pez"};
String[] english = {"America","tree","freedom","empathy","palm","wind","bird","thought","perseverance","dream","scream","jump","Sun","narwhal","anyone","dreams","cat","harmony","sea","indigenous","stone","net","view","subjective","do not","flower","beauty","ephemeral","theater","air","arrow","trust","peace","fool","patience","design","expression","upward","painting","crossing","rain","Stars","snail","nerve","heaven","action","space","kiss","nobility","idea","light","delusion","harmony","effort","light","flower","light","peace","creates","invent","free","surplus","boy","new","love","Sun","be reborn","creation","happiness","look","flower","far","freedom","Sun","dedication","fair","luck","light","light","air","dream","tree","respect","lifetime","universe","heaven","hope","new","serenity","happiness","sea","panic","cloud","poetry","fire","life","justice","Anglo","hope","lifetime","piano","Sun","effort","family","fight","verse","with you","tree","flora","Moon","hope","lifetime","height","amiability","nature","Sun","dream","path","expression","Odyssey","Ideas","change","new","walk","fool","origin","night","freedom","expansion","hybrid","the flock","asynchrony","high","ring","to fragment","the extension","the body","misalignment","extension","the profiles","impulse","battleship","Valley","elegance","segmented","the fist","link","sinuous","the flower","arrhythmia","disparate","the flight","distension","throb","the fish"};
String[] german = {"Amerika","Baum","Freiheit","Empathie","Palme","Wind","Vogel","Denken","Ausdauer","Traum","Schrei","springen","Sonne","Narwal","jeder","Tr\u00e4ume","Katze","Harmonie","Meer","autochthonen","Stein","Netzwerk","Ansicht","subjektiv","keine","Blume","Sch\u00f6nheit","kurzlebig","Theater","Luft","Pfeil","Vertrauen","Frieden","Dummkopf","Geduld","Design","Ausdruck","nach oben","Malerei","Kreuzung","regen","Sterne","Schnecke","Nerv","Himmel","Aktion","Raum","Kuss","Adel","Idee","Licht","Hoffnung","Harmonie","Anstrengung","Licht","Blume","Licht","Frieden","schafft","erfinden","kostenlos","\u00dcberschuss","Kind","neu","liebe","Sonne","wiedergeboren","Schaffung","Gl\u00fcck","aussehen","Blume","weit","Freiheit","Sonne","Widmung","Recht","Gl\u00fcck","Licht","Licht","Luft","Traum","Baum","Respekt","Leben","Universum","Himmel","Hoffnung","neu","Gelassenheit","Gl\u00fcck","Meer","Panik","Wolke","Poesie","Feuer","Leben","Gerechtigkeit","Anglo","Hoffnung","Leben","Klavier","Sonne","Anstrengung","Familie","Kampf","Vers","mit Ihnen","Baum","Flora","Mond","Hoffnung","Leben","H\u00f6he","Freundlichkeit","Natur","Sonne","Traum","Weg","Ausdruck","Odyssee","Ideen","Ver\u00e4nderung","neu","gehen","albern","Quelle","Nacht","Freiheit","Erweiterung","Hybrid","die Herde","Asynchronit\u00e4t","hoch","der Ring","fragmentieren","die Erweiterung","der K\u00f6rper","Fehlausrichtung","Erweiterung","die Profile","Impuls","gepanzert","das Tal","Eleganz","segmentiert","die Faust","Link","gewunden","die Blume","Herzrhythmusst\u00f6rungen","dispar","der Flug","aufgebl\u00e4ht","pochen","der Fisch"};
String[] greek = {"\u0391\u03bc\u03b5\u03c1\u03b9\u03ba\u03ae","\u03b4\u03ad\u03bd\u03c4\u03c1\u03bf","\u03b5\u03bb\u03b5\u03c5\u03b8\u03b5\u03c1\u03af\u03b1","\u03b5\u03bd\u03c3\u03c5\u03bd\u03b1\u03af\u03c3\u03b8\u03b7\u03c3\u03b7,\u03c3\u03c5\u03bc\u03c0\u03ac\u03b8\u03b5\u03b9\u03b1","\u03c0\u03b1\u03bb\u03ac\u03bc\u03b7","\u03ac\u03bd\u03b5\u03bc\u03bf\u03c2","\u03c0\u03bf\u03c5\u03bb\u03af","\u03c3\u03ba\u03ad\u03c8\u03b7","\u03b5\u03c0\u03b9\u03bc\u03bf\u03bd\u03ae","\u03cc\u03bd\u03b5\u03b9\u03c1\u03bf","\u03ba\u03c1\u03b1\u03c5\u03b3\u03ae","\u03ac\u03bb\u03bc\u03b1","\u03ae\u03bb\u03b9\u03bf\u03c2","\u03bc\u03bf\u03bd\u03cc\u03b4\u03bf\u03bd\u03c4\u03bf\u03c2 \u03c6\u03ac\u03bb\u03b1\u03b9\u03bd\u03b1","\u03ba\u03ac\u03b8\u03b5\u03bd\u03b1\u03c2 ","\u03cc\u03bd\u03b5\u03b9\u03c1\u03b1","\u03b3\u03ac\u03c4\u03b1","\u03b1\u03c1\u03bc\u03bf\u03bd\u03af\u03b1","\u03b8\u03ac\u03bb\u03b1\u03c3\u03c3\u03b1","\u03b1\u03c5\u03c4\u03cc\u03c7\u03b8\u03c9\u03bd","\u03c0\u03ad\u03c4\u03c1\u03b1","\u03b4\u03af\u03ba\u03c4\u03c5\u03bf","\u03b8\u03ad\u03b1","\u03c5\u03c0\u03bf\u03ba\u03b5\u03b9\u03bc\u03b5\u03bd\u03b9\u03ba\u03cc\u03c2","\u03cc\u03c7\u03b9","\u03bb\u03bf\u03c5\u03bb\u03bf\u03cd\u03b4\u03b9","\u03bf\u03bc\u03bf\u03c1\u03c6\u03b9\u03ac","\u03b5\u03c6\u03ae\u03bc\u03b5\u03c1\u03bf","\u03b8\u03ad\u03b1\u03c4\u03c1\u03bf","\u03b1\u03ad\u03c1\u03b1\u03c2","\u03b2\u03ad\u03bb\u03bf\u03c2","\u03b5\u03bc\u03c0\u03b9\u03c3\u03c4\u03bf\u03c3\u03cd\u03bd\u03b7","\u03b5\u03b9\u03c1\u03ae\u03bd\u03b7","\u03b1\u03bd\u03cc\u03b7\u03c4\u03bf\u03c2","\u03c5\u03c0\u03bf\u03bc\u03bf\u03bd\u03ae","\u03c3\u03c7\u03ad\u03b4\u03b9\u03bf","\u03ad\u03ba\u03c6\u03c1\u03b1\u03c3\u03b7","\u03b5\u03c0\u03ac\u03bd\u03c9","\u03b6\u03c9\u03b3\u03c1\u03b1\u03c6\u03b9\u03ba\u03ae","\u03b4\u03b9\u03ac\u03b2\u03b1\u03c3\u03b7","\u03b2\u03c1\u03bf\u03c7\u03ae","\u03b1\u03c3\u03c4\u03ad\u03c1\u03b9\u03b1","\u03c3\u03b1\u03bb\u03b9\u03b3\u03ba\u03ac\u03c1\u03b9","\u03bd\u03b5\u03cd\u03c1\u03bf","\u03bf\u03c5\u03c1\u03b1\u03bd\u03cc\u03c2","\u03b4\u03c1\u03ac\u03c3\u03b7","\u03c7\u03ce\u03c1\u03bf\u03c2","\u03c6\u03b9\u03bb\u03af","\u03b1\u03c1\u03c7\u03bf\u03bd\u03c4\u03b9\u03ac","\u03b9\u03b4\u03ad\u03b1","\u03c6\u03c9\u03c2","\u03b1\u03c5\u03c4\u03b1\u03c0\u03ac\u03c4\u03b7","\u03b1\u03c1\u03bc\u03bf\u03bd\u03af\u03b1","\u03c0\u03c1\u03bf\u03c3\u03c0\u03ac\u03b8\u03b5\u03b9\u03b1","\u03c6\u03c9\u03c2","\u03bb\u03bf\u03c5\u03bb\u03bf\u03cd\u03b4\u03b9","\u03c6\u03c9\u03c2","\u03b5\u03b9\u03c1\u03ae\u03bd\u03b7","\u03b4\u03b7\u03bc\u03b9\u03bf\u03c5\u03c1\u03b3\u03b5\u03af","\u03b5\u03c6\u03b5\u03c5\u03c1\u03af\u03c3\u03ba\u03c9","\u03b5\u03bb\u03b5\u03cd\u03b8\u03b5\u03c1\u03bf","\u03c0\u03bb\u03b5\u03cc\u03bd\u03b1\u03c3\u03bc\u03b1","\u03c0\u03b1\u03b9\u03b4\u03af","\u03bd\u03ad\u03bf\u03c2","\u03b1\u03b3\u03ac\u03c0\u03b7","\u03ae\u03bb\u03b9\u03bf\u03c2","\u03bd\u03b1 \u03be\u03b1\u03bd\u03b1\u03b3\u03b5\u03bd\u03bd\u03b7\u03b8\u03b5\u03af","\u03b4\u03b7\u03bc\u03b9\u03bf\u03c5\u03c1\u03b3\u03af\u03b1","\u03b5\u03c5\u03c4\u03c5\u03c7\u03af\u03b1","\u03bc\u03b1\u03c4\u03b9\u03ac","\u03bb\u03bf\u03c5\u03bb\u03bf\u03cd\u03b4\u03b9","\u03bc\u03b1\u03ba\u03c1\u03b9\u03ac","\u03b5\u03bb\u03b5\u03c5\u03b8\u03b5\u03c1\u03af\u03b1","\u03ae\u03bb\u03b9\u03bf\u03c2","\u03b1\u03c6\u03b9\u03ad\u03c1\u03c9\u03c3\u03b7","\u03b4\u03b5\u03be\u03b9\u03ac","\u03c4\u03cd\u03c7\u03b7","\u03c6\u03c9\u03c2","\u03c6\u03c9\u03c2","\u03b1\u03ad\u03c1\u03b1\u03c2","\u03cc\u03bd\u03b5\u03b9\u03c1\u03bf","\u03b4\u03ad\u03bd\u03c4\u03c1\u03bf","\u03c3\u03b5\u03b2\u03b1\u03c3\u03bc\u03cc\u03c2","\u03b6\u03c9\u03ae","\u03c3\u03cd\u03bc\u03c0\u03b1\u03bd","\u03bf\u03c5\u03c1\u03b1\u03bd\u03cc\u03c2","\u03b5\u03bb\u03c0\u03af\u03b4\u03b1","\u03bd\u03ad\u03bf\u03c2","\u03b3\u03b1\u03bb\u03ae\u03bd\u03b7","\u03b5\u03c5\u03c4\u03c5\u03c7\u03af\u03b1","\u03b8\u03ac\u03bb\u03b1\u03c3\u03c3\u03b1","\u03c0\u03b1\u03bd\u03b9\u03ba\u03cc\u03c2","\u03c3\u03cd\u03bd\u03bd\u03b5\u03c6\u03bf","\u03c0\u03bf\u03af\u03b7\u03c3\u03b7","\u03c6\u03c9\u03c4\u03b9\u03ac","\u03b6\u03c9\u03ae","\u03b4\u03b9\u03ba\u03b1\u03b9\u03bf\u03c3\u03cd\u03bd\u03b7","Anglo","\u03b5\u03bb\u03c0\u03af\u03b4\u03b1","\u03b6\u03c9\u03ae","\u03c0\u03b9\u03ac\u03bd\u03bf","\u03ae\u03bb\u03b9\u03bf\u03c2","\u03c0\u03c1\u03bf\u03c3\u03c0\u03ac\u03b8\u03b5\u03b9\u03b1","\u03bf\u03b9\u03ba\u03bf\u03b3\u03ad\u03bd\u03b5\u03b9\u03b1","\u03c0\u03ac\u03bb\u03b7","\u03c3\u03c4\u03af\u03c7\u03bf\u03c2","\u03bc\u03b1\u03b6\u03af \u03c3\u03bf\u03c5","\u03b4\u03ad\u03bd\u03c4\u03c1\u03bf","\u03c7\u03bb\u03c9\u03c1\u03af\u03b4\u03b1","\u03c6\u03b5\u03b3\u03b3\u03ac\u03c1\u03b9","\u03b5\u03bb\u03c0\u03af\u03b4\u03b1","\u03b6\u03c9\u03ae","\u03cd\u03c8\u03bf\u03c2","\u03ba\u03b1\u03bb\u03bf\u03c3\u03cd\u03bd\u03b7","\u03c6\u03cd\u03c3\u03b7","\u03ae\u03bb\u03b9\u03bf\u03c2","\u03cc\u03bd\u03b5\u03b9\u03c1\u03bf","\u03bc\u03bf\u03bd\u03bf\u03c0\u03ac\u03c4\u03b9","\u03ad\u03ba\u03c6\u03c1\u03b1\u03c3\u03b7","\u039f\u03b4\u03cd\u03c3\u03c3\u03b5\u03b9\u03b1","\u03b9\u03b4\u03ad\u03b5\u03c2","\u03b1\u03bb\u03bb\u03b1\u03b3\u03ae","\u03bd\u03ad\u03bf\u03c2","\u03b2\u03cc\u03bb\u03c4\u03b1","\u03b1\u03bd\u03cc\u03b7\u03c4\u03bf\u03c2","\u03c0\u03b7\u03b3\u03ae","\u03bd\u03cd\u03c7\u03c4\u03b1","\u03b5\u03bb\u03b5\u03c5\u03b8\u03b5\u03c1\u03af\u03b1","\u03b5\u03c0\u03ad\u03ba\u03c4\u03b1\u03c3\u03b7","\u03c5\u03b2\u03c1\u03b9\u03b4\u03b9\u03ba\u03cc","\u03c4\u03bf \u03ba\u03bf\u03c0\u03ac\u03b4\u03b9","\u03b1\u03c3\u03cd\u03b3\u03c7\u03c1\u03bf\u03bd\u03b7","\u03c8\u03b7\u03bb\u03ac","\u03c4\u03bf \u03b4\u03b1\u03c7\u03c4\u03c5\u03bb\u03af\u03b4\u03b9","\u03bd\u03b1 \u03b1\u03c0\u03bf\u03c3\u03c0\u03b1\u03c3\u03c4\u03b5\u03af","\u03c4\u03b7\u03bd \u03b5\u03c0\u03ad\u03ba\u03c4\u03b1\u03c3\u03b7","\u03c4\u03bf \u03c3\u03ce\u03bc\u03b1","\u03b5\u03c3\u03c6\u03b1\u03bb\u03bc\u03ad\u03bd\u03b7 \u03b5\u03c5\u03b8\u03c5\u03b3\u03c1\u03ac\u03bc\u03bc\u03b9\u03c3\u03b7","\u03bc\u03b5\u03b3\u03ad\u03b8\u03c5\u03bd\u03c3\u03b7","\u03c4\u03b1 \u03c0\u03c1\u03bf\u03c6\u03af\u03bb","\u03ce\u03b8\u03b7\u03c3\u03b7","\u03b8\u03c9\u03c1\u03b1\u03ba\u03b9\u03c3\u03bc\u03ad\u03bd\u03bf","\u03c4\u03b7\u03bd \u03ba\u03bf\u03b9\u03bb\u03ac\u03b4\u03b1","\u03ba\u03bf\u03bc\u03c8\u03cc\u03c4\u03b7\u03c4\u03b1","\u03ba\u03b1\u03c4\u03b1\u03ba\u03b5\u03c1\u03bc\u03b1\u03c4\u03b9\u03c3\u03bc\u03ad\u03bd\u03b7","\u03b7 \u03b3\u03c1\u03bf\u03b8\u03b9\u03ac","\u03c3\u03cd\u03bd\u03b4\u03b5\u03c3\u03bc\u03bf","\u03b5\u03bb\u03b9\u03ba\u03bf\u03b5\u03b9\u03b4\u03ae\u03c2","\u03c4\u03bf \u03bb\u03bf\u03c5\u03bb\u03bf\u03cd\u03b4\u03b9","\u03b1\u03c1\u03c1\u03c5\u03b8\u03bc\u03af\u03b1","\u03bd\u03b1 \u03b5\u03be\u03b1\u03c6\u03b1\u03bd\u03b9\u03c3\u03c4\u03b5\u03af","\u03c4\u03b7\u03bd \u03c0\u03c4\u03ae\u03c3\u03b7","\u03c6\u03bf\u03cd\u03c3\u03ba\u03c9\u03bc\u03b1","\u03c0\u03b1\u03bb\u03bc\u03cc\u03c2","\u03c4\u03b1 \u03c8\u03ac\u03c1\u03b9\u03b1"};
String[] french = {"am\u00e9rique","arbre","libert\u00e9","empathie","palme","vent","oiseau","penser","pers\u00e9v\u00e9rance","r\u00eave","crier","sauter","soleil","narval","tout","r\u00eaves","chat","harmonie","mer","autochtone","la pierre","rouge","vision","subjectif","non","fleur","beaut\u00e9","\u00e9ph\u00e9m\u00e8re","th\u00e9\u00e2tre","air","fl\u00e8che","confiance","la paix","zoperutano","patience","conception","expression","ascendant","peinture","croisement","pluie","\u00e9toiles","escargot","nerf","le paradis","action","l'espace","baiser","noblesse","id\u00e9e","lumi\u00e8re","illusion","harmonie","effort","lumi\u00e8re","fleur","lumi\u00e8re","la paix","cr\u00e9e","inventer","libre","restes","enfant","nouveau","amour","soleil","rena\u00eetre","cr\u00e9ation","f\u00e9licit\u00e9","regarder","fleur","loin","libert\u00e9","soleil","d\u00e9vouement","juste","la chance","lumi\u00e8re","lumi\u00e8re","air","r\u00eave","arbre","le respect","vie","univers","le paradis","l'espoir","nouveau","s\u00e9r\u00e9nit\u00e9","f\u00e9licit\u00e9","mer","panique","nuage","po\u00e9sie","le feu","vie","justice","Anglo","l'espoir","vie","piano","soleil","effort","famille","combattre","verset","avec vous","arbre","flore","lune","l'espoir","vie","hauteur","gentillesse","la nature","soleil","r\u00eave","route","expression","odyss\u00e9e","id\u00e9es","changer","nouveau","marcher","imb\u00e9cile","source","nuit","libert\u00e9","expansion","hybride","le troupeau","asynchronie","\u00e9lev\u00e9","l'anneau","se fragmenter","l'extension","le corps","d\u00e9salignement","\u00e9largissement","les profils","impulsion","blind\u00e9","la vall\u00e9e","\u00e9l\u00e9gance","segment\u00e9","le poing","lien","sinueux","la fleur","arythmie","dispar","le vol","ballonnement","palpiter","le poisson"};
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
