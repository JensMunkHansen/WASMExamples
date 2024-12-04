const repl = require('repl');
const createGL = require('gl');

// Initialize a WebGL context
const width = 800;
const height = 600;
const gl = createGL(width, height, { preserveDrawingBuffer: true });

// Start a REPL
const replServer = repl.start({
    prompt: 'webgl> ',
});

// Expose WebGL context to the REPL
replServer.context.gl = gl;
replServer.context.width = width;
replServer.context.height = height;

replServer.defineCommand('save', {
    help: 'Save the current WebGL frame to an image file',
    action(filename) {
        console.log(`Saving frame to ${filename}`);
        // Implement saving logic here
        this.displayPrompt();
    },
});

console.log('WebGL REPL started. You can use the "gl" object to interact with the WebGL context.');
console.log('Example: gl.clearColor(0.0, 0.0, 1.0, 1.0); gl.clear(gl.COLOR_BUFFER_BIT);');
