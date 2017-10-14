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
    tinc = random(-0.005, 0.005);

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

  void render() {

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
      ellipse(x, y, 2.1 * radius, 2.1 * radius);
    }
  }
}