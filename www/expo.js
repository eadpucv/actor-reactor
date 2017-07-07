var data;

function setup(){
	createCanvas(windowWidth, windowHeight);
	noStroke();
	data = [];
}

function draw(){
	background(0);
	updateData();
	
	var d = width / 4;
	var h = height / 4;

	drawGeneric(d, h, data[0]);
	drawGeneric(width/2, h, data[1]);
	drawGeneric(width-d, h, data[2]);

	drawGeneric(d, height/2, data[3]);
	drawGeneric(width/2, height/2, data[4]);
	drawGeneric(width-d, height/2, data[5]);

	drawGeneric(d, height - h, data[6]);
	drawGeneric(width/2, height - h, data[7]);
	drawGeneric(width-d, height - h, data[8]);
}

function updateData(){
	for (var i = 0; i < 9; i++){
		data[i] = noise(i + millis()/900);
	}
	println(data[0]);
}

function drawGeneric(x, y, value){
	push();
	translate(x, y);
    // draw cross
    stroke(255);
    line(-50, -50, 50, 50);
    line(-50, 50, 50, -50);

    // blob
    noStroke();
	fill(200, 222, 10, 90);
	ellipse(0, 0, value * 600, value * 600);
	pop();
}

function mousePressed(){

}

function windowResized(){
	setup();
}