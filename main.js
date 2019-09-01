const WIDTH = 320;
const HEIGHT = 240;
const BUFFER_SIZE_OUT = WIDTH * HEIGHT * 4;
const canvas = /** @type {HTMLCanvasElement} */ (document.getElementById(
  "canvas"
));
canvas.width = WIDTH;
canvas.height = HEIGHT;
const ctx = canvas.getContext("2d");

const frameBufferOut = new Uint8ClampedArray(BUFFER_SIZE_OUT);
/** @type {WebAssembly.Memory} */
let memory;
/** @type {Uint16Array} */
let frameBufferIn;

/**
 * @param {{[name: string]: any}} imports 
 */
async function init(imports) {
  const buffer = await fetch("./test.wasm").then(r => r.arrayBuffer());
  const { instance } = await WebAssembly.instantiate(buffer, { env: imports });
  memory = instance.exports.memory;
  frameBufferIn = new Uint16Array(
    instance.exports.memory.buffer,
    instance.exports.get_frame_buffer(),
    WIDTH * HEIGHT
  );
  
  instance.exports.init();
  requestAnimationFrame(function tick() {
    instance.exports.on_frame();
    requestAnimationFrame(tick);
  });
}

function updateCanvas() {
  if (!frameBufferIn) return;
  for (let x = 0; x < WIDTH; x++) {
    for (let y = 0; y < HEIGHT; y++) {
      const valIn = frameBufferIn[y * WIDTH + x];
      const r = (valIn >> 11) & 0b011111;  // 5 bits
      const g = (valIn >>  5) & 0b111111;  // 6 bits
      const b = (valIn >>  0) & 0b011111;  // 5 bits
      const i = 4 * (y * WIDTH + x);
      frameBufferOut[i + 0] = r << 3;
      frameBufferOut[i + 1] = g << 2;
      frameBufferOut[i + 2] = b << 3;
      frameBufferOut[i + 3] = 255;
    }
  }
  const imageData = new ImageData(frameBufferOut, WIDTH, HEIGHT);
  ctx.putImageData(imageData, 0, 0);
}

const utf8decoder = new TextDecoder("utf-8");
/**
 * @param {number} str 
 * @param {number} len 
 */
function print(str, len) {
  if (!memory) return;
  const bytes = new Uint8Array(memory.buffer, str, len);
  console.log(bytes.join(", "), utf8decoder.decode(bytes));
}

init({ flush_frame: updateCanvas, print });
