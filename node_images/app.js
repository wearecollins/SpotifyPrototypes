var fs = require('fs')
  , gm = require('./gm');

var colorA = {
	r: Math.floor(random() * 255),
	g: Math.floor(random() * 255),
	b: Math.floor(random() * 255)
}, colorB = {
	r: Math.floor(random() * 255),
	g: Math.floor(random() * 255),
	b: Math.floor(random() * 255)
}

function setColor( toSet, newColor ){
	toSet.r = newColor.r;
	toSet.g = newColor.g;
	toSet.b = newColor.b;
}

function process( img ){
	var newImg = img.substr(0,img.length-4) +"_edit" + img.substr(img.length-4);
	gm(img)
	.view()
	.write(newImg, function (err) {
	  if (!err) console.log('done');
	});
}

// Command line args
var program = require('commander');

program
  .version('0.0.1')
  .option('-i, --image', 'Image to load')
  .option('-a, --colora', 'Color one in format r,g,b')
  .option('-b, --colorb', 'Color two in format r,g,b')
  .parse(process.argv);

if ( program.colora ){
	var split = program.colora.split(',');
	if ( split.length >= 3){
		var c = {r:split[0], g:split[1], b:split[2]}
		setColor(colorA, c);
	}
}
if ( program.colorb ){
	var split = program.colorb.split(',');
	if ( split.length >= 3){
		var c = {r:split[0], g:split[1], b:split[2]}
		setColor(colorB, c);
	}
}

if (program.image){
	process(img);
} else {
	console.error("No image passed, returning!");
}