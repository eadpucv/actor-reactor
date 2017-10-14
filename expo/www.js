/**
 *     Visualizing Actor Reactor Data - Exhibit Monitor
 */

var data;   			// the dynamic data

var hu, vu, units;      // horizontal and vertical units
var xu, yu;	    // horizontal and vertical units
var hm, vm; 			// horizontal and vertical margins
var gvm;				// global vertical margin (for title)

var font;

function setup() {

  font = loadFont('lib/Montserrat-Regular.otf');
  textFont(font, 14);


  // proportions
  var modWidth = 8;
  var modHeight = 5.9; 
  var proportion = modHeight/modWidth;

  var myCanvas = createCanvas(windowWidth, windowHeight);
  myCanvas.parent('underlay');
  myCanvas.id('all-beasts');


  hu = 5;       // horizontal units
  vu = 2;       // vertical units
  units = hu * vu;

  data = [];

  gvm = windowHeight * .333;  // global vertical margin
  vm = windowHeight / 5;      // vertical margin
  hm = windowWidth  / 5;      // horizontal margin

  xu = (width - hm*2)  / (hu - 1);
  yu = xu * proportion;
}

function draw() {
  background(0);
  updateData();

  var i = 0;

  for (var y = 0; y < 2; y ++) {
    for (var x = 0; x < 5; x ++) {

      var xpos = hm + x*xu;
      var ypos = vm*2 + y*yu;

      drawGrid(i, xpos, ypos);
      drawGridUnit(i, xpos, ypos);

      i++;
    }
  }
}

function drawGrid(n, x, y) {
  push();
  translate(x,y);
  var textPadding = 16;
  fill(255, 150);
  text(n, - xu/2 + textPadding, yu/2 - textPadding * 1.5);
  stroke(255);
  noFill();
  
  strokeWeight(1);
  ellipse(-xu/2, -yu/2, 5, 5);
  ellipse(xu/2, -yu/2, 10, 10);
  ellipse(xu/2, yu/2, 15, 15);

  stroke(255, 100);
  rect(-xu/2, -yu/2, xu, yu);
  // line(-xu/2, -yu/2, xu/2, -yu/2);
  // line(-xu/2, -yu/2, -xu/2, yu/2);
  pop();
}

function drawGridUnit(n, x, y) {
  var textPadding = 16;
  push();
  translate(x,y);
  switch (n) {
    case 0:
    fill(255);
    text("entrada", -xu/2 + textPadding, -yu/2 + textPadding * 1.5);
    stroke(255);
    strokeWeight(3);
    noFill();
    arc(xu/2, - 10, 20, 20, HALF_PI, PI);
    arc(xu/2, + 10, 20, 20, PI, HALF_PI * 3);
    break;

    case 1:
    stroke(255);
    strokeWeight(3);
    line(-xu/5, -yu*.4, -xu/5, yu/3);
    line(-xu/5, -yu*.4, 0, -yu*.4);
    drawGeneric(0, 0, 150, data[0]);
    break;

    case 2:
    stroke(255);
    strokeWeight(3);
    line(xu/2 - xu/5, -yu*.4, xu/2 + xu/5, -yu*.4);
    line(xu/2, -yu*.4, xu/2, yu/3);
    strokeWeight(3);
    drawGeneric(-xu/2, 0, 50, data[1],);
    drawGeneric(xu*.33, -10, 150, data[2],);
    break;

    case 3:

    drawGeneric(-xu*.4, yu*.3, 90, data[6]);
    drawGeneric(xu*.25, -yu*.2, 60, data[7]);
    break;

    case 4:
    stroke(255);
    strokeWeight(3);
    line(xu/2 - xu/5, -yu*.4, xu/2, -yu*.4);
    line(xu/2, -yu*.4, xu/2, yu/3);
    strokeWeight(3);
    drawGeneric(0, 0, 200, data[8]);
    break;

    case 6:
    fill(255);
    text("salida", -xu/2 + textPadding, - yu/2 + textPadding * 1.5);
    noStroke();
    fill(255, 40);
    rect(-xu/2 + 20, yu*.2, 100, 25);
    drawGeneric(20, 0, 100, data[3]);
    scale(1, -1);
    stroke(255);
    strokeWeight(3);
    line(xu/2 - xu/5, -yu*.4, xu/2 + xu/5, -yu*.4);
    line(xu/2, -yu*.4, xu/2, yu/3);
    strokeWeight(3);
    break;

    case 7:
    drawGeneric(-xu*.35, -yu*.1, 66, data[4]);
    break;

    case 8:
    stroke(255);
    drawGeneric(0, 0, 120, data[5]);
    scale(1, -1);
    strokeWeight(3);
    line(xu/2 - xu/5, -yu*.4, xu/2, -yu*.4);
    line(xu/2, -yu*.4, xu/2, yu/3);
    strokeWeight(3);
    break;

    case 9:
    rectMode(CENTER);
    fill(255, 40);
    rect(0, 0, xu*.9, yu*.9);
    break;

  }
  pop();
}

function updateData() {
  for (var i = 0; i < units; i++) {
    data[i] = noise(i + millis()/900);
  }
}

function drawGeneric(x, y, amp, value) {
  push();
  translate(x, y);
  rotate(value * 6.29);

  // blob
  noStroke();
  fill(200, 222, 10, 90);
  ellipse(0, 0, 10 + value * amp, 10 + value * amp);

  // draw cross
  stroke(0);
  strokeWeight(1);
  line(-amp/10, -amp/10, amp/10, amp/10);
  line(-amp/10, amp/10, amp/10, -amp/10);
  pop();
}

function mousePressed() {
}

function windowResized() {
  setup();
}
