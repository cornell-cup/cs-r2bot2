var canvas = document.getElementById("canvas");
var ctx = canvas.getContext("2d");
var md = false;
var rad = 250;
var diff = 90;
var smallrad = 45;
var w = canvas.width;
var h = canvas.height;
var textX = Math.floor(w * .07);
var textY = Math.floor(h * .1);
var dataCoordX = 0;
var dataCoordY = 0;

function getOffset(el) {
  el = el.getBoundingClientRect();
  return {
    left: el.left + window.scrollX,
    top: el.top + window.scrollY
  }
}

var canx = getOffset(canvas).left;
var cany = getOffset(canvas).top;
var cx = Math.floor(w * .35);
var cy = Math.floor(h * .35);

function circle(x,y,r) {
 ctx.beginPath();
 ctx.arc(x,y,r,0,2*Math.PI);
 ctx.closePath();
 ctx.stroke();
}

function clear() {
 ctx.clearRect(0, 0, w, h);
}

function collide(c1x, c1y, c1radius, c2x, c2y, c2radius)
{
    var dX = Math.pow(c1x - c2x, 2);
    var dY = Math.pow(c1y - c2y, 2);
    var r2 = Math.pow(c1radius - c2radius, 2) + 5;
    if (dX + dY > r2){
    	return [Math.abs(cx-c2x), Math.abs(cy-c2y)];
    }
    else {
    	return [0, 0];
    }
}

function drawInner(x,y,r){
	clear(0,0,w,h);
  ctx.beginPath();
  var coords = collide(cx,cy,rad,x,y,r);
  if (coords === [0,0]){
  	ctx.arc(x,y,r,0,2*Math.PI);
  }
  else{
  	if (x < cx && y < cy){
    	ctx.arc(x+coords[0], y+coords[1], r, 0, 2*Math.PI);
    }
    else if (x > cx && y < cy){
    	ctx.arc(x-coords[0], y+coords[1], r, 0, 2*Math.PI);
    }
    else if (x < cx && y > cy){
    	ctx.arc(x+coords[0], y-coords[1], r, 0, 2*Math.PI);
    }
    else if (x > cx && y > cy){
    	ctx.arc(x-coords[0], y-coords[1], r, 0, 2*Math.PI);
    }
    else{
    	ctx.arc(x,y,r,0,2*Math.PI);
    }
  }
 	ctx.closePath();
 	ctx.stroke();
}

function init() {
 drawInner(cx,cy,smallrad);
 circle(cx,cy,rad-diff);
 ctx.strokeText("("+ 0 + ", " + 0 + ")", textX, textY);
}

canvas.addEventListener("mousedown", function(e) {
  md = true;
  drawInner(e.clientX-canx, e.clientY-cany, smallrad);
  circle(cx, cy, rad - diff);
  dataCoordX = e.clientX - cx - canx;
  dataCoordY = cy - e.clientY + cany;
  ctx.strokeText("("+ (e.clientX-cx-canx) + ", " + (cy-e.clientY+cany) + ")", textX, textY);
});

canvas.addEventListener("mousemove", function(e) {
  if (md) {
  	drawInner(e.clientX-canx, e.clientY-cany, smallrad);
  	circle(cx, cy, rad - diff);
  	dataCoordX = e.clientX - cx - canx;
  	dataCoordY = cy - e.clientY + cany;
    ctx.strokeText("("+ (e.clientX-cx-canx) + ", " + (cy-e.clientY+cany) + ")", textX, textY);
  }
});

canvas.addEventListener("mouseup", function(e) {
  md = false;
  drawInner(cx,cy,smallrad);
  circle(cx, cy, rad - diff);
  dataCoordX = 0;
  dataCoordY = 0;
  ctx.strokeText("("+ 0 + ", " + 0 + ")", textX, textY);
});

var touchX, touchY;

function getTouchPos(e) {
    if (!e)
        var e = event;

    if (e.touches) {
        if (e.touches.length == 1) { // Only deal with one finger
            var touch = e.touches[0]; // Get the information for finger #1
            touchX = Math.floor(touch.pageX);
            touchY = Math.floor(touch.pageY);
        }
    }
}

canvas.addEventListener("touchstart", function(e) {
  e.preventDefault();
  getTouchPos(e);
  drawInner(touchX-canx, touchY-cany, smallrad);
  circle(cx, cy, rad - diff);
  dataCoordX = touchX - cx - canx;
  dataCoordY = cy - touchY + cany;
  ctx.strokeText("("+ (touchX-cx-canx) + ", " + (cy-touchY+cany) + ")", textX, textY);
});

canvas.addEventListener("touchmove", function(e) {
  e.preventDefault();
  getTouchPos(e);
  drawInner(touchX-canx, touchY-cany, smallrad);
  circle(cx, cy, rad - diff);
  dataCoordX = touchX - cx - canx;
  dataCoordY = cy - touchY + cany;
  ctx.strokeText("("+ (touchX-cx-canx) + ", " + (cy-touchY+cany) + ")", textX, textY);
});

canvas.addEventListener("touchend", function(e) {
  e.preventDefault();
  drawInner(cx,cy,smallrad);
  circle(cx, cy, rad - diff);
  dataCoordX = 0;
  dataCoordY = 0;
  ctx.strokeText("("+ 0 + ", " + 0 + ")", textX, textY);
});

init();