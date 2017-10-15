class Point {
  int seed;
  float amp, zoom;
  float x, y, t, s; // x,y, angle and step
  
  Point(float x, float y) {
    this.x = x;
    this.y = y;
    t = random(TWO_PI);
    s = random(-.2, .2);
    
    seed = round(random(100000));
    amp = random(.2, .5);
    zoom = random(400, 1000);
  }

  void move() {
    
    noiseSeed(seed);
    float dif = .5 - noise((float)millis()/zoom) * amp;
    t += dif;
    
    x += cos(t)*s;
    y += sin(t)*s;
  }
}