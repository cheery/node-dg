var dg = require('./build/Release/dg');

exports.api = {
    webgl: 'dg-webgl'
    //webgl: '../node-dg-webgl/build/Release/webgl'
};

var getContext = function(name) {
    var module = exports.api[name];
    if (!module) throw "" + name + " not found";
    var api = require(module);
    return new api.Renderer(this.id, this.id);
};

var surfaceClose = function() {
    dg.destroySurface(this.id);
};

var fullscreenClose = function() {
    dg.destroySurface(this.id);
    dg.closeDisplayBRCM(this.display);
};

exports.getFullscreen = function(options) {
    if (!options) options = {};
    var display = dg.openDisplayBRCM(0);
    var screen = dg.fullscreenResolutionBRCM(display);
    var id = dg.createFullscreenSurfaceBRCM(display, 0, 0, screen.width, screen.height, 0, options);
    var width = screen.width;
    var height = screen.height;
    if (options.width != undefined) width = options.width;
    if (options.height != undefined) height = options.height;
    return {
        display: display,
        getContext: getContext,
        close: fullscreenClose,
        width: width,
        height: height,
        id: id
    };
};

exports.createSurface = function(width, height, options) {
    if (!options) options = {};
    var id = dg.createSurface(width, height, options);
    return {
        getContext: getContext,
        close: surfaceClose,
        width: width,
        height: height,
        id: id
    };
};

exports.surfaceId = dg.surfaceId;
