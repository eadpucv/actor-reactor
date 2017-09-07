class Point {
  float x, y, t, s; // x,y, angle and step
  Point(float x, float y) {
    this.x = x;
    this.y = y;
    t = random(TWO_PI);
    s = random(0, 1);
  }

  void move() {
    x += cos(t)*s;
    y += sin(t)*s;
  }
}