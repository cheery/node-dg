# Bindings for Direct Graphics API

Word of warning: This is still bit sharp edged module. I advice to stay calm if everything doesn't work.

# How To Install

 * Install https://github.com/cheery/dg according to instructions.
 * npm install node-dg # or directly from this repository.

# How To Use

    var dg = require('dg');
    var surface = dg.getFullscreen({});
    var gl = surface.getContext('webgl');
