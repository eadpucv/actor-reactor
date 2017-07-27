/**
*     Visualizing Actor Reactor Data - Exhibit Monitor
*/

var data;   			// the dynamic data

var hu, vu, units;      // horizontal and vertical units
var spacerX, spacerY;	// horizontal and vertical spacers
var hm, vm; 			// horizontal and vertical margins
var gvm;				// global vertical margin (for title)

function setup(){
	createCanvas(windowWidth, windowHeight);
	
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
			drawGeneric(x, y, data[i]);
			i++;
		}
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