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
    maxalpha = 50;  // maximum line opacity
  }

  void reset() {
    lifespan = round(random(70, 250));
    existence = lifespan * 1.5;          // lifespan + blacktime
    tic = 0;
    fade = 255;
    numLines = round(random(3, 6));
    lin = new Line[numLines];
    int count = 0;
    int lang = int(random(4));
    randomWord = round(random(spanish.length - 1));
    bitmap = createGraphics(int(radius * 3), int(radius * 3));


    bitmap.beginDraw();
    {
      bitmap.textFont(font);
      bitmap.pushMatrix();
      {
        bitmap.translate(x, y);
        bitmap.noFill();
        bitmap.stroke(white);
        bitmap.strokeWeight(5);
        ellipse(0, 0, radius, radius);

        bitmap.fill(red);

        bitmap.text("spanish", 0, 0);
        bitmap.text("english", 0, -radius);
        bitmap.text("german", -radius/2, radius);
        bitmap.text("greek", 0, radius);

        bitmap.fill(255);

        switch(lang) {
        case 0: /* spanish */
          bitmap.text(spanish[randomWord], 0, 0);
          break;
        case 1: /* english */
          bitmap.text(english[randomWord], 0, -radius);
          break;
        case 2: /* german */
          bitmap.text(german[randomWord], -radius/2, radius);
          break;
        case 3: /* greek */
          bitmap.textFont(greekFont);
          bitmap.text(greek[randomWord], 0, radius);
          break;
        }
      }
      bitmap.popMatrix();
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

  void updateGraphics() {
    bitmap.beginDraw();
    alpha = sin((float)tic*PI/(float)lifespan) * maxalpha;
    if (tic < lifespan) {
      for (int i = 0; i < lin.length; i++) {
        lin[i].render(bitmap, alpha);
      }
    } 
    bitmap.endDraw();
  }


  void render() {
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