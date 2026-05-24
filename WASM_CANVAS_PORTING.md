# WebAssembly Canvas Port

This port adds a browser target without using SDL.  It keeps the existing Dreamcast, SDL 1.2, SDL3, and headless paths in place and adds one more backend set:

- `src/renderer/wasm_canvas/renderer_wasm_canvas.c`
- `src/input/wasm_canvas/input_wasm_canvas.c`
- `src/filesystem/wasm_canvas/filesystem_wasm_canvas.c`
- `src/platform/wasm_canvas/platform_wasm_canvas.c`
- `Makefile.wasm`
- `web/wasm_canvas_shell.html`

The gameplay code still talks to `renderer.h`, `input.h`, `audio.h`, `filesystem.h`, and `platform.h`.  The browser-specific state is private to the backend C files.

## Build

```sh
make -f Makefile.wasm
```

The output is written to `build/wasm/` as `index.html`, `index.js`, `index.wasm`, and the Emscripten data package.

Serve it over HTTP:

```sh
make -f Makefile.wasm serve
```

Then open `http://localhost:8000/`.

## Toolchain

This target uses Emscripten's LLVM/clang compiler frontend.  It does not link SDL.  Emscripten is still used because this game expects libc, file loading, and a browser runtime; the actual video path is a small Canvas bridge rather than SDL's browser backend.

## Rendering

The C renderer owns a 400×240 RGBA software framebuffer when `WIDESCREEN` is set.  `Renderer_RefreshScreen()` copies that buffer into a Canvas `ImageData` object using JavaScript glue emitted from the C backend.

The canvas element keeps its intrinsic game resolution.  Upscaling is deliberately left to CSS:

```css
#gns-canvas {
  width: min(100vw, calc(100vh * var(--gns-aspect)));
  height: auto;
  image-rendering: pixelated;
}
```

This preserves the game coordinate system and avoids baking scale factors into gameplay or renderer code.

## Input

Keyboard state is collected by browser `keydown` and `keyup` handlers and exposed to C as a compact polling API.  The default mapping matches the SDL backend:

- Arrow keys or keypad 2/4/6/8: movement
- Left Ctrl or X: primary button
- Left Alt, Space, or C: secondary button
- Enter: third button
- F1/F2: volume down/up
- Escape: exit

## Files and saves

Bitmap and map data are loaded from Emscripten's preloaded virtual filesystem:

```make
--preload-file data/$(DATASET)/image@image
--preload-file data/data@data
```

Save/config data are stored through `localStorage` under `ganbare-natsuki-san:*` keys.

## Timing

The original scene functions contain blocking loops.  The WASM target therefore compiles with Asyncify and implements `Renderer_Delay()` with `emscripten_sleep()`.  This gives the browser event loop time to repaint and process input without rewriting every scene into a state-machine step function.

## Audio

The first WASM Canvas target uses the existing headless audio backend and defines `NOSOUND`.  That keeps the initial browser port independent of browser autoplay policy and lets video/input/gameplay be validated first.  A later audio backend can map `audio.h` onto WebAudio.

## Update: preferred clang path

The preferred browser build is now `Makefile.wasm` / `Makefile.wasm-clang`, which uses upstream clang/LLD and a frame-stepped browser host. The earlier Emscripten/Asyncify build has been retained as `Makefile.wasm-emcc`.
