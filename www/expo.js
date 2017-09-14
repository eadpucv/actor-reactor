/**
*     Visualizing Actor Reactor Data - Exhibit Monitor
*/

var data;   			// the dynamic data

var hu, vu, units;      // horizontal and vertical units
var spacerX, spacerY;	// horizontal and vertical spacers
var hm, vm; 			// horizontal and vertical margins
var gvm;				// global vertical margin (for title)

var layout;
var font;

function setup(){
	layout = [];
	font = loadFont('lib/Montserrat-Regular.otf');
	textFont(font, 14);
	layout[0] = true;
	layout[1] = true;
	layout[2] = true;
	layout[3] = true;
	layout[4] = true;
	layout[5] = false;
	layout[6] = false;
	layout[7] = false;
	layout[8] = true;
	layout[9] = false;
	layout[10] = true;
	layout[11] = false;
	layout[12] = false;
	layout[13] = true;
	layout[14] = true;
	layout[15] = true;
	layout[16] = false;
	layout[17] = false;

	var myCanvas = createCanvas(windowWidth, windowHeight);
	myCanvas.parent('underlay');
	myCanvas.id('all-beasts');

	if(windowWidth > windowHeight){
		hu = 6;
		vu = 3; 
	}else{
		hu = 3;
		vu = 6; 
	}

	units = hu * vu;
	data = [];
	gvm = windowHeight / 3;
	vm = windowHeight / 5;
	hm = windowWidth  / 7;
	spacerX = (width - hm*2)  / (hu-1);
	spacerY = (height - vm - gvm) / (vu-1);
}

function draw(){
	background(0);
	
	updateData();
	
	var i = 0;

	for(var y = gvm; y <= height; y += spacerY){
		for(var x = hm; x < width; x += spacerX){
			drawPillars(x, y);
			drawGridNumber(i, x, y);
			if(layout[i] == true){	
				drawGeneric(x, y, data[i]);
			}
			i++;
		}
	}
}

function drawPillars(x, y){
	var pin = 3;
	push();
		stroke(255);
		noFill();
		strokeWeight(1);
		ellipse(x - spacerX/2, y - spacerY/2, pin * 2, pin * 2);
		ellipse(x + spacerX/2, y - spacerY/2, pin * 4, pin * 4);
		ellipse(x + spacerX/2, y + spacerY/2, pin * 8, pin * 8);

		stroke(255, 100);
		line(x - spacerX/2, y - spacerY/2, x + spacerX/2, y - spacerY/2);
		line(x - spacerX/2, y - spacerY/2, x - spacerX/2, y + spacerY/2);
	pop();
}

function drawGridNumber(n, x, y){
	var textPadding = 16;
	fill(255, 150);
	text(n, x - spacerX/2 + textPadding, y + spacerY/2 - textPadding * 1.5);
	switch (n)
	{
		case 1:
		push();
			fill(255);
			text("entrada", x - spacerX/2 + textPadding, y - spacerY/2 + textPadding * 1.5);
			stroke(255);
			strokeWeight(3);
			line(x, y + spacerY/2, x + spacerX/2, y + spacerY/2);
		pop();
		break;
		
		case 2:
		push();
		strokeWeight(3);
			stroke(255);
			line(x - spacerX/2, y + spacerY/2, x + spacerX/2, y + spacerY/2);
		pop();
		break;
		
		case 3:
		push();
			strokeWeight(3);
			stroke(255);
			line(x - spacerX/2, y + spacerY/2, x + spacerX/2, y + spacerY/2);
		pop();
		break;

		
		case 7:
		push();
			fill(255);
			text("salida", x - spacerX/2 + textPadding, y - spacerY/2 + textPadding * 1.5);
		pop();
		break;

		case 8:
		push();
			strokeWeight(3);
			stroke(255);
			line(x - spacerX/2, y - spacerY/2, x - spacerX/2, y + spacerY/2);
		pop();
		break;

		case 15:
		push();
			strokeWeight(3);
			stroke(255);
			line(x - spacerX/2, y - spacerY/2, x - spacerX/2, y + spacerY/2);
		pop();
		break;

	}

}

function updateData(){
	for (var i = 0; i < units; i++){
		data[i] = noise(i + millis()/900);
	}
	// println(data[0]);
}

function drawGeneric(x, y, value){
	push();
	translate(x, y);
	rotate(value * 6.29);

    // blob
    noStroke();
    fill(200, 222, 10, 90);
    ellipse(0, 0, 200 + value * 50, 200 + value * 50);

	// draw cross
	stroke(0);
	line(-50, -50, 50, 50);
	line(-50, 50, 50, -50);
	
	
	pop();
}

function mousePressed(){

}

function windowResized(){
	setup();
}