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
	layout[4] = false;
	layout[5] = false;
	layout[6] = true;
	layout[7] = true;
	layout[8] = false;
	layout[9] = true;
	layout[10] = true;
	layout[11] = true;

	var myCanvas = createCanvas(windowWidth, windowHeight);
	myCanvas.parent('underlay');
	myCanvas.id('all-beasts');

	if(windowWidth > windowHeight){
		hu = 5;
		vu = 3; 
	}else{
		hu = 3;
		vu = 5; 
	}

	units = hu * vu;
	data = [];
	gvm = windowHeight / 3;
	vm = windowHeight / 5;
	hm = windowWidth  / 7;
	spacerX = (width - 2*hm)  / (hu-1);
	spacerY = (height - vm - gvm) / (vu-1);
}

function draw(){
	background(0);
	
	updateData();
	
	var i = 0;

	for(var y = gvm; y <= height; y += spacerY){
		for(var x = hm; x <= width; x += spacerX){
			if(layout[i] == true){
				drawPillars(x, y);
				drawGeneric(x, y, data[i]);
			}
			drawGridNumber(i, x, y);
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
		case 0:
		push();
			fill(255);
			text("entrada", x - spacerX/2 + textPadding, y - spacerY/2 + textPadding * 1.5);
		pop();
		break;
		
		case 1:
		push();
		strokeWeight(3);
			stroke(255);
			line(x - spacerX/2, y + spacerY/2, x + spacerX/2, y + spacerY/2);
		pop();
		break;
		
		case 2:
		push();
			strokeWeight(3);
			stroke(255);
			line(x - spacerX/2, y + spacerY/2, x + spacerX/2, y + spacerY/2);
		pop();
		
		case 2:
		break; 

		case 3:
		break;

		case 4:
		break;

		case 5:
		break;

		case 6:
		push();
			strokeWeight(3);
			stroke(255);
			line(x - spacerX/2, y - spacerY/2, x - spacerX/2, y + spacerY/2);
		pop();
		break;

		case 7:
		break;

		case 8:
		break;

		case 9:
		break;

		case 10:
		push();
			fill(255);
			text("salida", x - spacerX/2 + textPadding, y - spacerY/2 + textPadding * 1.5);
		pop();
		break;

		case 11:
		break;

		case 12:
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