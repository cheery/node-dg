var dg = require('./dg');

var surface = dg.getFullscreen({});
var gl = surface.getContext('webgl');

var randomColor = function () {
    return {
        r: Math.random(),
        g: Math.random(),
        b: Math.random(),
        a: Math.random()
    };
};

var lastColor = randomColor();
var nextColor = randomColor();
var lastTime = Date.now();
var nextTime = Date.now() + 500;

var loop = function () {
    var i = (Date.now() - lastTime) / (nextTime - lastTime);
    if (i >= 1.0) {
        lastColor = nextColor;
        nextColor = randomColor();
        lastTime = nextTime;
        nextTime += 500;
        i = 0.0;
    }
    var j = 1-i;

    gl.clearColor(
        lastColor.r*j + nextColor.r*i,
        lastColor.g*j + nextColor.g*i,
        lastColor.b*j + nextColor.b*i,
        lastColor.a*j + nextColor.a*i
    );
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.swapBuffers();
}

setInterval(loop, 1000/30);
