import initModule from './emscripten-generated.mjs';

const Module = await initModule({
  canvas: document.getElementById('my-canvas')
});

