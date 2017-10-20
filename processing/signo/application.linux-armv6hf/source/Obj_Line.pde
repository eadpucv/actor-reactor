class Line {
  Point p1, p2, c1, c2;
  boolean curve;
  color c;
  float sw, t, tinc;

  Line(int a, int b) {

    p1 = new Point(pt[a].x, pt[a].y);
    p2 = new Point(pt[b].x, pt[b].y);

    p1.s = random(.1, .5);
    p2.s = random(.1, .5);
    
    t = random(TWO_PI);
    tinc = random(-0.0002, 0.0002);

    c = strokeColor[round(random(2))];
    sw = random(.5, 3);

    if (random(1) > .5) { // curve-straight probability
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
      float A = 0.8;
      float B = 1 - A;
      c1 = new Point(p1.x * A + p2.x * B, p1.y * A + p2.y * B);
      c2 = new Point(p1.x * B + p2.x * A, p1.y * B + p2.y * A);
    }
    
    c1.s = random(.5, 1.5);
    c2.s = random(.5, 1.5);
  }

  void render(PGraphics bm, float a) {

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