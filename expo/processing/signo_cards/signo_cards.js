/*

    cartas de la phalène ~ documenta 14 ~ Atenas
    
    amereida

*/


var pt;
var sides;
var radius;
var signo;
var looping;
var debugtext;
var randomtext;
var font, fs;
var rot;
var portrait;

function preload() {
	font = "Helvetica, Arial";
}

function orient(){
	if (width > height) {
		portarit = false;
		radius = height / 3;
		fs = height / 18;
	} else {
		portrait = true;
		radius = width / 3;
		fs = width / 18;
	}
	for (var i = 0; i < 9; i++) {
		var inc = TWO_PI / 9;

		// 9 points (pt) distributed in circle (circular identity)
		var xpos = cos(i * inc) * radius;
		var ypos = sin(i * inc) * radius;
		pt[i] = [];
		pt[i][0] = xpos;
		pt[i][1] = ypos;
	}
}

function windowResized() {
	resizeCanvas(windowWidth, windowHeight);
	orient();
	background(0);
	strokeWeight(2);
	stroke(0);
	signo.render();
	drawText();
}

function setup() {
	var carta = createCanvas(windowWidth, windowHeight);
	carta.parent('card');
	frameRate(18);
	sides = 9;
	pt = [];
	looping = true;
	rot = 0;
	orient();
	textFont(font, fs);
	if(portrait){
		signo = new Signo(width / 2, height / 3);
	} else {
		signo = new Signo(width / 2, height / 2);
	}
	debugtext = "";
}

function Signo(x, y) {
	this.x = x;
	this.y = y;
	largo = round(random(3, 6));
	var lin = [];
	var count = 0;
	var start, end;

	debugText = "el signo tiene " + largo + " trazos";

	while (count < largo) {

		start = round(random(sides - 1));
		end = round(random(sides - 1));

		if (start != end) {
			lin[count] = new Linea(start, end);
			count++;
		}
	}

	this.render = function() {
		push();
		translate(x, y);
		rotate(rot);
		for (var i = 0; i < lin.length; i++) {
			lin[i].render();
		}
		pop();
	}
}

function Linea(a, b) {
	var x1, x2, y1, y2, cx1, cy1, cx2, cy2;
	var curve;

	x1 = pt[a][0];
	y1 = pt[a][1];
	x2 = pt[b][0];
	y2 = pt[b][1];

	if (random(1) > .5) { // curve-straight probability
		curve = true;
	} else {
		curve = false;
	}

	if (curve) {
		var d = dist(x1, y1, x2, y2);
		var amp = map(d, 0, radius * 2, 1, 0);
		cx1 = x1 * amp + random(-d / 4, d / 4);
		cy1 = y1 * amp + random(-d / 4, d / 4);
		cx2 = x2 * amp + random(-d / 4, d / 4);
		cy2 = y2 * amp + random(-d / 4, d / 4);
	}

	this.render = function() {
		if (curve) {
			bezier(x1, y1, cx1, cy1, cx2, cy2, x2, y2);
		} else {
			line(x1, y1, x2, y2);
		}
	}
}

function draw() {
	noFill();
	if (looping) {
		if(portrait){
			signo = new Signo(width / 2, height / 3);
		}else{
			signo = new Signo(width / 2, height / 2);
		}
		strokeWeight(5);
		stroke(255, 50);
		signo.render();
		strokeWeight(1);
		stroke(255);
		signo.render();
		fill(0, 20);
		noStroke();
		rect(0, 0, width, height);
		rot += .002;
	} else {
		background(0);
		strokeWeight(2);
		stroke(255);
		signo.render();
		drawText();
	}
}

function drawText() {
	fill(255);
	noStroke();
	textAlign(CENTER);
	push();
	translate(width / 2, height / 2);

	// english - top (180º)
	push();
	translate(0, -height / 2 + fs);
	rotate(PI);
	text(english[randomText], 0, 0);
	pop();

	// spanish - bottom (0º)
	push();
	translate(0, height / 2 - fs);
	text(spanish[randomText], 0, 0);
	pop();

	// greek - left (90º)
	push();
	translate(-width / 2 + fs, 0);
	rotate(HALF_PI);
	text(greek[randomText], 0, 0);
	pop();

	// german - right (-90º)
	push();
	translate(width / 2 - fs, 0);
	rotate(-HALF_PI);
	text(german[randomText], 0, 0);
	pop();
	pop();

}

function mouseReleased() {
	looping = !looping;
	randomText = round(random(spanish.length - 1));
	/*
	if (!looping) {
		saveCanvas('myCanvas', 'jpg');
	}*/
}