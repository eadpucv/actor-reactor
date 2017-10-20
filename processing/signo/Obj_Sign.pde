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

  void reset() {
    init = true;
    lifespan = round(random(70, 250));
    existence = lifespan * 1.5;          // lifespan + blacktime
    tic = 0;
    fade = 255;
    numLines = round(random(3, 6));
    lin = new Line[numLines];
    int count = 0;

    randomWord = round(random(spanish.length - 1));
    bitmap = createGraphics(int(radius * 3), int(radius * 3));
    while (count < numLines) {
      start = round(random(sides - 1));
      end = round(random(sides - 1));
      if (start != end) {
        lin[count] = new Line(start, end);
        count++;
      }
    }
  }


  void drawText() {
    
    int lang = int(random(5));
    
    bitmap.textFont(font);
    bitmap.fill(white);

    float k = radius * .666;
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

  void updateGraphics() {
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