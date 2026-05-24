# WASM Canvas port using upstream clang/LLD

This tree contains two browser-oriented Canvas experiments:

- `Makefile.wasm` / `Makefile.wasm-clang`: the preferred target. It uses upstream `clang` and `wasm-ld` through the `wasm32-unknown-unknown` target, a tiny freestanding runtime, and a JavaScript host for Canvas, input, Web Audio, assets, and `localStorage` saves.
- `Makefile.wasm-emcc`: the earlier Emscripten/Asyncify experiment, retained for comparison.

The SDL 1.2, SDL3, Dreamcast, and headless backends are not removed.

## Build

```sh
make -f Makefile.wasm
```

Equivalent explicit form:

```sh
make -f Makefile.wasm-clang
```

Serve the generated browser package:

```sh
make -f Makefile.wasm serve
```

Then open `http://localhost:8000/`.

The build output is written to:

```text
build/wasm-clang/index.html
build/wasm-clang/gns_wasm.js
build/wasm-clang/ganbare_natsuki_san.wasm
build/wasm-clang/assets.bin
build/wasm-clang/assets.json
```

## Toolchain model

This target intentionally avoids Emscripten, SDL, WASI, and Asyncify. It uses:

```text
clang --target=wasm32-unknown-unknown
lld's WebAssembly linker path via clang
-nostdlib / -ffreestanding
```

The port provides only the small libc subset this game currently needs in:

```text
src/wasm/freestanding/include/
src/wasm/freestanding/wasm_runtime.c
```

That runtime supplies allocation, memory/string routines, basic formatting, `rand`, `time`, and exit/log hooks. Browser-specific work is kept outside libc and goes through explicit imports implemented by `web/wasm_clang/gns_wasm.js`.

## Frame-stepped execution

The earlier Emscripten route used Asyncify because the original game scenes are blocking loops. The clang target instead adds a compile-time frame-stepped mode:

```text
-DGNS_FRAME_STEPPED
```

In that mode the C side exports:

```c
int  gns_start(void);
int  gns_frame(void);
void gns_shutdown(void);
```

The browser owns `requestAnimationFrame()` and calls `gns_frame()` once per frame. Scene functions retain their old blocking `*_main()` implementations for SDL/Dreamcast builds, but compile alternative `*_step()` functions for the browser target.

This makes the browser port asynchronous-friendly without rewriting the gameplay rules or changing the legacy codepaths.

## Canvas and CSS scaling

The renderer remains a software framebuffer backend. The C renderer writes a 400x240 RGBA buffer, and JavaScript copies it to Canvas `ImageData`. The displayed size is controlled by CSS:

```css
#gns-canvas {
  width: min(100vw, calc(100vh * var(--gns-aspect)));
  aspect-ratio: var(--gns-aspect);
  image-rendering: pixelated;
}
```

The internal framebuffer is still the game's widescreen mode. Browser upscaling is CSS-only.

## Keyboard mapping

The browser target accepts both the old SDL-style mapping and a more natural web mapping:

```text
Arrow keys or WASD  move
Z or Enter          confirm / button A
X or Space          cancel / button X
C                   button Y
V or Shift          button B
Tab                 select
F1 / F2             volume down / up
Escape              exit
```

Input is now event-driven rather than mask-polled. JavaScript keyboard, pointer, and Gamepad events call the exported C function:

```c
void gns_input_button_event(int button, int pressed);
```

The C backend owns the pressed-state array and a separate latched push array. This means a quick key tap is still seen even if keydown and keyup both occur between two browser animation frames. It also avoids JavaScript bitmask conversion and keeps all `Input_IsPress()` / `Input_IsPush()` semantics in C.

The WASM build also normalizes the logical game control bindings to the default GP2X button indices at startup. This prevents an older browser `localStorage` config, written by a previous experimental build, from leaving the game with stale or partial control mappings.

The page includes on-screen controls and browser Gamepad API polling as fallback input paths. For quick browser-side input diagnosis, append `?debugInput=1` to the page URL. The status line will show the current virtual button names.

The old always-visible control summary has been replaced with a collapsed `Controls` panel so it does not cover the game framebuffer during play. Opening the panel exposes a remapper for both keyboard and Gamepad API devices. Keyboard bindings are stored as `KeyboardEvent.key`-oriented tokens so AZERTY and other layouts follow the key labels users see. Gamepad bindings support both standard buttons and axis directions such as `Axis 0 +` or `Axis 1 -`. Remaps are persisted in `localStorage` under a versioned key and can be reset from the panel.

A first key press, pointer click, or gamepad input also resumes the Web Audio context, which is required by normal browser autoplay policy.

## Mobile and desktop touch testing

On phones and tablets, the browser page shows an on-screen controller automatically. The left side is an analog-style virtual joystick; dragging it translates to the existing UP/DOWN/LEFT/RIGHT GP2X button state, including diagonals. The right side exposes A, X, Y, and B action buttons, with small START and SELECT buttons near the bottom center. This layer is browser-only and does not alter SDL, SDL3, Dreamcast, or headless input code.

Desktop browsers usually report a fine pointer, so the touch overlay is hidden by default. To test the phone controls on desktop, serve the build and append `?touch=1`:

```text
http://localhost:8000/?touch=1
```

You can then drag the virtual joystick with the mouse and click the action buttons. Add `&debugInput=1` as well to show the virtual GP2X buttons currently held by the browser input layer:

```text
http://localhost:8000/?touch=1&debugInput=1
```

## Audio

The clang target now uses a raw browser audio backend rather than the headless audio backend:

```text
src/audio/wasm_raw/audio_wasm_raw.c
web/wasm_clang/gns_wasm.js
```

The C backend keeps the existing opaque `audio.h` API. It imports small host calls such as `gns_audio_load_bgm`, `gns_audio_play_bgm`, `gns_audio_load_se`, and `gns_audio_play_se`. The JavaScript host decodes the packed `.ogg` BGM and `.wav` sound effects with Web Audio, routes BGM and SE through separate gain nodes, and resumes audio from the first user gesture.

Because Web Audio decoding is asynchronous, C-side load calls register slots synchronously, while the JS side finishes decoding in the background. If the game requests BGM before decoding has completed, playback is started once the decode finishes.

## Assets and saves

`tools/pack_wasm_assets.py` packs:

```text
data/wide/image -> image/
data/data       -> data/
data/sound      -> sound/
```

into `assets.bin` plus `assets.json`. The C file, renderer, and audio backends read assets through JS host imports and the packed manifest. Asset reads are size-based: map files and save-style data may request only the byte count used by the original C arrays, while BMP/audio loads request the full packed asset.

Save/config files are mapped to `localStorage` by the JS host through:

```c
int gns_local_storage_load(const char *key, unsigned char *dst, int size);
int gns_local_storage_save(const char *key, const unsigned char *src, int size);
```

If `work.sav` is missing, corrupt, or unavailable because browser storage is blocked, the WASM filesystem supplies a validated fresh stage-1 work save in memory. This prevents the ACT loader from falling through to the ending scene when a first-run browser profile has no persisted save data. Missing `item_wk.sav` is treated as an empty item state.

## Current limitations

The freestanding libc is intentionally narrow. It is enough for the current selected source set, but it is not a general-purpose libc.

The browser audio path depends on browser support for the shipped audio formats. WAV sound effects are broadly supported; Ogg Vorbis BGM is supported in most modern desktop browsers, but some WebKit/Safari versions may require transcoded music assets.


## Input timing note

The clang/wasm Canvas target uses a browser-event-driven input path. JavaScript
posts key, pointer, and gamepad transitions into C with `gns_input_button_event()`.
The C backend snapshots those raw events once at the start of each exported
`gns_frame()` call. `Input_IsPush()` then remains true for that entire game
frame, matching the legacy SDL input semantics, while held keys only generate
one push edge until released and pressed again.

`FPSWait()` and legacy `KeyInput()` calls deliberately do not call `Input_Update()` in `GNS_FRAME_STEPPED` builds, because scene logic runs before those calls in much of the original code. The exported `gns_frame()` function is the single input-snapshot point for browser builds.

## Keyboard-layout note

The browser input bridge intentionally reads `KeyboardEvent.key` before
`KeyboardEvent.code` for printable controls.  This is important on AZERTY and
other non-QWERTY layouts: a physical key can report `code="KeyW"` while the
visible key and intended control is `key="z"`.  Confirm/cancel controls should
therefore follow the character printed on the user's keyboard, while Arrow keys
remain layout-independent.

## Audio prompt behavior

The browser build now uses a separate `#audio-prompt` overlay instead of leaving
"Click or press a key to enable audio" in the status line. The prompt is shown
while the Web Audio context is absent/suspended. It disappears after a key,
pointer, or gamepad gesture resumes the context and the game volume is non-zero.
If the in-game volume is muted, the prompt changes to an explicit muted-state
message rather than claiming that a click is still required.

## Optional Newgrounds medals

The clang/WASM build includes optional Newgrounds.io medal hooks without adding
any dependency to SDL, SDL3, Dreamcast, or headless builds. The C side calls a
single browser import, `gns_host_achievement(event_id, value)`, for WASM-only
achievement events. The JavaScript host maps those events to local achievement
toasts and, when configured, to Newgrounds `Medal.unlock` calls.

The included events are:

```text
startup        game reached gns_start()
first_level    Stage 1 completed in normal play
game_complete  Stage 50 completed in normal play
```

Newgrounds support is disabled by default because app IDs, AES keys, and medal
IDs are project-specific. See `web/wasm_clang/NEWGROUNDS.md` and
`web/wasm_clang/newgrounds_config.example.js`. Local toasts use thumbnails in
`web/wasm_clang/achievements/`, generated from bundled game art. Newgrounds medal
icons still need to be configured on the Newgrounds project page.

## Pause control behavior

The browser default binding now treats `Escape` and `P` as `Pause / Start`, not
as an exit command.  The old `Escape -> EXIT` binding was hostile to browser play:
players naturally pressed Escape to pause and could end up stopping the exported
frame loop.  The input-map localStorage key was bumped so existing stale browser
bindings are reset once.

The in-game pause menu also accepts the same `Pause / Start` control to resume
play.  This is intentionally WASM-friendly behavior for keyboard, touch, and
Gamepad API users; OK/Cancel still work through the original menu path.

## Widescreen background generation

`data/wide/image/bak/*.bmp` are generated 400x240 assets.  The source 4:3
backgrounds are scaled up while preserving aspect ratio, then center-cropped to
fill the 400x240 framebuffer.  For 320x240 assets this means scaling to 400x300
and cropping 30 pixels from the top and bottom.  The generator also applies a
small unsharp pass to avoid a soft resampled look.

Regenerate them with:

```sh
python3 tools/generate_wide_backgrounds.py
```

This is an asset-preparation step, not part of the normal `make -f Makefile.wasm`
path, because it depends on Pillow.


## Default browser controls

Keyboard defaults are intentionally browser/game-friendly and avoid modifier keys. Use Arrow keys or WASD to move, Z or Enter for Confirm/A, X or Space for Cancel/Jump/X, P or Escape for Pause/Menu/Y, C or V for B, Q/E for L/R, Tab for Select, and F1/F2 or -/= for volume. These can be remapped from the in-page Controls panel; the mapping is stored in browser localStorage.


### Debug level cheat

For testing only, open the browser build with `?debug=1`:

```
http://127.0.0.1:8000/?debug=1
```

A small `Debug cheats` panel appears under the fullscreen/pause buttons. Pick a level from 1 to 50 and press `Start level` to jump directly into that normal stage.

The cheat is WASM-only and does not write to the normal browser save slot. It uses an in-memory work-save override, disables Time Attack score eligibility, and suppresses save-file achievement fallback while the debug run is active.
