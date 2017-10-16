class Line {
  Point p1, p2, c1, c2;
  boolean curve;
  color c;
  float sw, t, tinc;

  Line(int a, int b) {

    p1 = new Point(pt[a].x, pt[a].y);
    p2 = new Point(pt[b].x, pt[b].y);

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
    }
  }

  void render(PGraphics bitmap, float a) {
    
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