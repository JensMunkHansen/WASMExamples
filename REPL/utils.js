// utils.js
function saveToImage(gl, width, height, filename) {
    const fs = require('fs');
    const { PNG } = require('pngjs');

    const pixels = new Uint8Array(width * height * 4);
    gl.readPixels(0, 0, width, height, gl.RGBA, gl.UNSIGNED_BYTE, pixels);

    const png = new PNG({ width, height });
    png.data = pixels;

    png.pack().pipe(fs.createWriteStream(filename));
    console.log(`Saved render to ${filename}`);
}

module.exports = { saveToImage };
