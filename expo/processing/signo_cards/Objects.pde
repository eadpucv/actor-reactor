

class Signo {
  float x, y;
  int largo;
  Linea[] lin;
  int count, start, end;

  Signo (float x, float y) {
    this.x = x;
    this.y = y;
    largo = round(random(3, 6));
    lin = new Linea[largo];
    count = 0;

    debugText = "el signo tiene " + largo + " trazos";

    while (count < largo) {

      start = round(random(sides - 1));
      end = round(random(sides - 1));

      if (start != end) {
        lin[count] = new Linea(start, end);
        count++;
      }
    }
  }

  void render() {
    pushMatrix();
    translate(x, y);
    rotate(rot);
    for (int i = 0; i < lin.length; i++) {
      lin[i].render();
    }
    popMatrix();
  }
}

class Linea {
  float x1, x2, y1, y2, cx1, cy1, cx2, cy2;
  boolean curve;

  Linea(int a, int b) {

    x1 = pt[a].x;
    y1 = pt[a].y;
    x2 = pt[b].x;
    y2 = pt[b].y;

    if (random(1) > .5) { // curve-straight probability
      curve = true;
    } else {
      curve = false;
    }

    if (curve) {
      float d = dist(x1, y1, x2, y2);
      float amp = map(d, 0, radius * 2, 1, 0);
      cx1 = x1 * amp + random(-d / 4, d / 4);
      cy1 = y1 * amp + random(-d / 4, d / 4);
      cx2 = x2 * amp + random(-d / 4, d / 4);
      cy2 = y2 * amp + random(-d / 4, d / 4);
    }
  }

  void render() {
    if (curve) {
      bezier(x1, y1, cx1, cy1, cx2, cy2, x2, y2);
    } else {
      line(x1, y1, x2, y2);
    }
  }
}