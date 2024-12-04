const fs = require('fs');
const { PNG } = require('pngjs');

// Save the current WebGL context to an image file
function saveToImage(gl, width, height, filename) {
    const pixels = new Uint8Array(width * height * 4);
    gl.readPixels(0, 0, width, height, gl.RGBA, gl.UNSIGNED_BYTE, pixels);

    const png = new PNG({ width, height });
    png.data = pixels;

    png.pack().pipe(fs.createWriteStream(filename));
    console.log(`Saved render to ${filename}`);
}

// Example usage in REPL:
// saveToImage(gl, width, height, 'output.png');
replServer.context.saveToImage = saveToImage;
