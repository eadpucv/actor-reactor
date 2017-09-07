class Line {
  Point c1, c2;
  float x1, x2, y1, y2, cx1, cy1, cx2, cy2;
  boolean curve;
  color c;
  float sw;

  Line(int a, int b) {

    x1 = pt[a].x;
    y1 = pt[a].y;
    x2 = pt[b].x;
    y2 = pt[b].y;

    c = strokeColor[round(random(2))];
    sw = random(.5, 2);

    if (random(1) > .5) { // curve-straight probability
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

  void render(float a) {
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