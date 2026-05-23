(() => {
  'use strict';

  const canvas = document.getElementById('gns-canvas');
  const status = document.getElementById('status');
  const controls = document.getElementById('controls');
  let instance = null;
  let memory = null;
  let imageData = null;
  let ctx = null;
  let assetBytes = null;
  let assetManifest = null;
  let running = false;

  const DEBUG_INPUT = new URLSearchParams(window.location.search).has('debugInput');

  const GP2X_BUTTON = Object.freeze({
    UP: 0,
    DOWN: 4,
    LEFT: 2,
    RIGHT: 6,
    CLICK: 18,
    A: 12,
    B: 13,
    Y: 14,
    X: 15,
    L: 10,
    R: 11,
    START: 8,
    SELECT: 9,
    VOLUP: 16,
    VOLDOWN: 17,
    EXIT: 19
  });

  const GP2X_BUTTON_NAME = Object.freeze({
    UP: GP2X_BUTTON.UP,
    DOWN: GP2X_BUTTON.DOWN,
    LEFT: GP2X_BUTTON.LEFT,
    RIGHT: GP2X_BUTTON.RIGHT,
    A: GP2X_BUTTON.A,
    B: GP2X_BUTTON.B,
    Y: GP2X_BUTTON.Y,
    X: GP2X_BUTTON.X,
    L: GP2X_BUTTON.L,
    R: GP2X_BUTTON.R,
    START: GP2X_BUTTON.START,
    SELECT: GP2X_BUTTON.SELECT,
    VOLUP: GP2X_BUTTON.VOLUP,
    VOLDOWN: GP2X_BUTTON.VOLDOWN,
    EXIT: GP2X_BUTTON.EXIT
  });

  const GAME_ACTIONS = Object.freeze([
    { id: 'UP', label: 'Up', button: GP2X_BUTTON.UP },
    { id: 'DOWN', label: 'Down', button: GP2X_BUTTON.DOWN },
    { id: 'LEFT', label: 'Left', button: GP2X_BUTTON.LEFT },
    { id: 'RIGHT', label: 'Right', button: GP2X_BUTTON.RIGHT },
    { id: 'A', label: 'Confirm / A', button: GP2X_BUTTON.A },
    { id: 'X', label: 'Cancel / Jump / X', button: GP2X_BUTTON.X },
    { id: 'Y', label: 'Third action / Y', button: GP2X_BUTTON.Y },
    { id: 'B', label: 'B', button: GP2X_BUTTON.B },
    { id: 'L', label: 'L', button: GP2X_BUTTON.L },
    { id: 'R', label: 'R', button: GP2X_BUTTON.R },
    { id: 'START', label: 'Start', button: GP2X_BUTTON.START },
    { id: 'SELECT', label: 'Select', button: GP2X_BUTTON.SELECT },
    { id: 'VOLDOWN', label: 'Volume down', button: GP2X_BUTTON.VOLDOWN },
    { id: 'VOLUP', label: 'Volume up', button: GP2X_BUTTON.VOLUP },
    { id: 'EXIT', label: 'Exit', button: GP2X_BUTTON.EXIT }
  ]);

  const ACTION_BY_ID = Object.freeze(Object.fromEntries(GAME_ACTIONS.map((action) => [action.id, action])));
  const REMAP_STORAGE_KEY = 'ganbare-natsuki-san:wasm-input-map-v3';

  const DEFAULT_BINDINGS = Object.freeze({
    keyboard: Object.freeze({
      UP: Object.freeze(['keyboard:key:ArrowUp', 'keyboard:key:w', 'keyboard:key:8']),
      DOWN: Object.freeze(['keyboard:key:ArrowDown', 'keyboard:key:s', 'keyboard:key:2']),
      LEFT: Object.freeze(['keyboard:key:ArrowLeft', 'keyboard:key:a', 'keyboard:key:4']),
      RIGHT: Object.freeze(['keyboard:key:ArrowRight', 'keyboard:key:d', 'keyboard:key:6']),
      A: Object.freeze(['keyboard:key:z', 'keyboard:key:Enter', 'keyboard:key:Control']),
      X: Object.freeze(['keyboard:key:x', 'keyboard:key: ', 'keyboard:key:Alt', 'keyboard:key:Backspace']),
      Y: Object.freeze(['keyboard:key:c']),
      B: Object.freeze(['keyboard:key:v', 'keyboard:key:Shift']),
      L: Object.freeze([]),
      R: Object.freeze([]),
      START: Object.freeze([]),
      SELECT: Object.freeze(['keyboard:key:Tab']),
      VOLDOWN: Object.freeze(['keyboard:key:F1', 'keyboard:key:-']),
      VOLUP: Object.freeze(['keyboard:key:F2', 'keyboard:key:=']),
      EXIT: Object.freeze(['keyboard:key:Escape', 'keyboard:key:Esc'])
    }),
    gamepad: Object.freeze({
      UP: Object.freeze(['gamepad:button:12', 'gamepad:axis:1:-']),
      DOWN: Object.freeze(['gamepad:button:13', 'gamepad:axis:1:+']),
      LEFT: Object.freeze(['gamepad:button:14', 'gamepad:axis:0:-']),
      RIGHT: Object.freeze(['gamepad:button:15', 'gamepad:axis:0:+']),
      A: Object.freeze(['gamepad:button:0']),
      X: Object.freeze(['gamepad:button:1']),
      Y: Object.freeze(['gamepad:button:2']),
      B: Object.freeze(['gamepad:button:3']),
      L: Object.freeze(['gamepad:button:4', 'gamepad:button:6']),
      R: Object.freeze(['gamepad:button:5', 'gamepad:button:7']),
      START: Object.freeze(['gamepad:button:9']),
      SELECT: Object.freeze(['gamepad:button:8']),
      VOLDOWN: Object.freeze([]),
      VOLUP: Object.freeze([]),
      EXIT: Object.freeze([])
    })
  });

  const buttonNamesForDebug = Object.freeze([
    ['UP', GP2X_BUTTON.UP],
    ['DOWN', GP2X_BUTTON.DOWN],
    ['LEFT', GP2X_BUTTON.LEFT],
    ['RIGHT', GP2X_BUTTON.RIGHT],
    ['A', GP2X_BUTTON.A],
    ['X', GP2X_BUTTON.X],
    ['Y', GP2X_BUTTON.Y],
    ['B', GP2X_BUTTON.B],
    ['L', GP2X_BUTTON.L],
    ['R', GP2X_BUTTON.R],
    ['START', GP2X_BUTTON.START],
    ['SELECT', GP2X_BUTTON.SELECT],
    ['VOL-', GP2X_BUTTON.VOLDOWN],
    ['VOL+', GP2X_BUTTON.VOLUP],
    ['EXIT', GP2X_BUTTON.EXIT]
  ]);

  const sourceButtons = new Map();
  const buttonHolders = Array.from({ length: 20 }, () => new Set());

  function setStatus(text) {
    if (status) status.textContent = text || '';
  }

  function u8() {
    return new Uint8Array(memory.buffer);
  }

  function cstr(ptr) {
    const heap = u8();
    let end = ptr;
    while (heap[end] !== 0) end++;
    return new TextDecoder('utf-8').decode(heap.subarray(ptr, end));
  }

  function normalizePath(path) {
    return path.replace(/\\/g, '/').replace(/^\.\//, '');
  }

  function assetEntryFromPath(path) {
    const normal = normalizePath(path);
    return assetManifest && assetManifest.files ? assetManifest.files[normal] || null : null;
  }

  function assetEntry(ptr) {
    return assetEntryFromPath(cstr(ptr));
  }

  function assetBytesForPath(path) {
    const entry = assetEntryFromPath(path);
    if (!entry) return null;
    return assetBytes.subarray(entry.offset, entry.offset + entry.size);
  }

  function localStorageKey(ptr) {
    return 'ganbare-natsuki-san:' + cstr(ptr);
  }

  function b64FromBytes(bytes) {
    let s = '';
    const chunk = 0x8000;
    for (let i = 0; i < bytes.length; i += chunk) {
      s += String.fromCharCode.apply(null, bytes.subarray(i, i + chunk));
    }
    return btoa(s);
  }

  function bytesFromB64(value) {
    const s = atob(value);
    const out = new Uint8Array(s.length);
    for (let i = 0; i < s.length; i++) out[i] = s.charCodeAt(i) & 255;
    return out;
  }

  function makeAudioHost() {
    const host = {
      ctx: null,
      masterGain: null,
      bgmGain: null,
      seGain: null,
      bgm: [],
      se: [],
      currentBgm: null,
      activeSe: [],
      bgmVol: 1,
      seVol: 1,
      masterVol: 1,
      wantedBgm: -1
    };

    function AudioContextCtor() {
      return window.AudioContext || window.webkitAudioContext || null;
    }

    function volGain(vol, master) {
      const v = Math.max(0, Math.min(128, Number(vol) || 0)) / 128;
      const m = Math.max(0, Number(master) || 0) / 100;
      return v * m;
    }

    function ensure() {
      if (host.ctx) return true;
      const Ctor = AudioContextCtor();
      if (!Ctor) {
        setStatus('WebAudio is not available in this browser.');
        return false;
      }
      host.ctx = new Ctor();
      host.masterGain = host.ctx.createGain();
      host.bgmGain = host.ctx.createGain();
      host.seGain = host.ctx.createGain();
      host.masterGain.gain.value = host.masterVol;
      host.bgmGain.gain.value = host.bgmVol;
      host.seGain.gain.value = host.seVol;
      host.bgmGain.connect(host.masterGain);
      host.seGain.connect(host.masterGain);
      host.masterGain.connect(host.ctx.destination);
      return true;
    }

    function resume() {
      if (!ensure()) return;
      if (host.ctx.state === 'suspended') {
        host.ctx.resume().catch(() => {});
      }
    }

    function decodeSlot(slot) {
      if (!slot || slot.buffer || slot.promise) return slot ? slot.promise : null;
      if (!ensure()) return null;
      const bytes = assetBytesForPath(slot.path);
      if (!bytes) {
        console.warn(`missing audio asset: ${slot.path}`);
        return null;
      }
      const copy = bytes.buffer.slice(bytes.byteOffset, bytes.byteOffset + bytes.byteLength);
      slot.promise = host.ctx.decodeAudioData(copy).then((buffer) => {
        slot.buffer = buffer;
        slot.promise = null;
        if (slot.pendingBgm) {
          slot.pendingBgm = false;
          startBgm(slot.index);
        }
        return buffer;
      }).catch((err) => {
        slot.promise = null;
        console.warn(`could not decode ${slot.path}`, err);
        return null;
      });
      return slot.promise;
    }

    function stopBgm() {
      if (host.currentBgm && host.currentBgm.source) {
        try { host.currentBgm.source.stop(); } catch (_) {}
      }
      host.currentBgm = null;
      host.wantedBgm = -1;
    }

    function startBgm(index) {
      if (!ensure()) return -1;
      const slot = host.bgm[index];
      if (!slot) return -1;
      host.wantedBgm = index;
      if (!slot.buffer) {
        slot.pendingBgm = true;
        decodeSlot(slot);
        return 0;
      }
      stopBgm();
      const source = host.ctx.createBufferSource();
      source.buffer = slot.buffer;
      source.loop = slot.loop !== 0;
      source.connect(host.bgmGain);
      host.currentBgm = { index, source };
      source.onended = () => {
        if (host.currentBgm && host.currentBgm.source === source) host.currentBgm = null;
      };
      try {
        source.start();
      } catch (err) {
        console.warn('could not start BGM', err);
        host.currentBgm = null;
        return -1;
      }
      resume();
      return 0;
    }

    function playSe(index) {
      if (!ensure()) return -1;
      const slot = host.se[index];
      if (!slot) return -1;
      if (!slot.buffer) {
        decodeSlot(slot);
        return 0;
      }
      const source = host.ctx.createBufferSource();
      source.buffer = slot.buffer;
      source.connect(host.seGain);
      const active = { index, source };
      host.activeSe.push(active);
      source.onended = () => {
        const pos = host.activeSe.indexOf(active);
        if (pos >= 0) host.activeSe.splice(pos, 1);
      };
      try {
        source.start();
      } catch (err) {
        console.warn('could not start sound effect', err);
        return -1;
      }
      resume();
      return 0;
    }

    function stopSe(index) {
      for (let i = host.activeSe.length - 1; i >= 0; i--) {
        const active = host.activeSe[i];
        if (index < 0 || active.index === index) {
          try { active.source.stop(); } catch (_) {}
          host.activeSe.splice(i, 1);
        }
      }
    }

    return {
      unlock: resume,
      init() {
        /* Do not require a gesture here; browsers usually create a suspended
           context and allow resume from the first key/pointer event. */
        ensure();
        return 0;
      },
      shutdown() {
        stopBgm();
        stopSe(-1);
        if (host.ctx) {
          host.ctx.close().catch(() => {});
        }
        host.ctx = null;
      },
      loadBgm(num, pathPtr, loop) {
        const path = cstr(pathPtr);
        const entry = assetEntryFromPath(path);
        if (!entry) {
          console.warn(`missing BGM asset: ${path}`);
          return -1;
        }
        host.bgm[num] = { index: num, path, loop, buffer: null, promise: null, pendingBgm: false };
        decodeSlot(host.bgm[num]);
        return 0;
      },
      loadSe(num, pathPtr) {
        const path = cstr(pathPtr);
        const entry = assetEntryFromPath(path);
        if (!entry) {
          console.warn(`missing SE asset: ${path}`);
          return -1;
        }
        host.se[num] = { index: num, path, loop: 0, buffer: null, promise: null, pendingBgm: false };
        decodeSlot(host.se[num]);
        return 0;
      },
      playBgm: startBgm,
      stopBgm,
      isBgmPlaying() {
        return host.currentBgm ? 1 : 0;
      },
      setBgmGain(vol, master) {
        host.bgmVol = volGain(vol, master);
        if (host.bgmGain) host.bgmGain.gain.value = host.bgmVol;
      },
      setSeGain(vol, master) {
        host.seVol = volGain(vol, master);
        if (host.seGain) host.seGain.gain.value = host.seVol;
      },
      playSe,
      stopSe,
      stopSeAll() {
        stopSe(-1);
      },
      isSePlaying(num) {
        return host.activeSe.some((active) => active.index === num) ? 1 : 0;
      },
      pause() {
        if (host.ctx && host.ctx.state === 'running') host.ctx.suspend().catch(() => {});
      },
      resume
    };
  }

  const audioHost = makeAudioHost();

  function wasmInputEvent(button, down) {
    const exports = instance && instance.exports;
    if (exports && exports.gns_input_button_event) {
      exports.gns_input_button_event(button | 0, down ? 1 : 0);
    }
  }

  function wasmInputClear() {
    const exports = instance && instance.exports;
    if (exports && exports.gns_input_clear) {
      exports.gns_input_clear();
    }
  }

  let inputBindings = loadInputBindings();
  let captureBinding = null;
  let captureGamepadIgnore = new Set();
  let captureTimeoutId = 0;
  const suppressedKeyboardTokens = new Set();
  const gamepadSourceSet = new Set();

  function cloneBindingsFrom(src) {
    const out = { keyboard: {}, gamepad: {} };
    for (const type of ['keyboard', 'gamepad']) {
      for (const action of GAME_ACTIONS) {
        const list = src && src[type] && Array.isArray(src[type][action.id]) ? src[type][action.id] : [];
        out[type][action.id] = list.filter((token) => typeof token === 'string');
      }
    }
    return out;
  }

  function loadInputBindings() {
    try {
      const raw = localStorage.getItem(REMAP_STORAGE_KEY);
      if (raw) return cloneBindingsFrom(JSON.parse(raw));
    } catch (err) {
      console.warn('could not read input bindings', err);
    }
    return cloneBindingsFrom(DEFAULT_BINDINGS);
  }

  function saveInputBindings() {
    try {
      localStorage.setItem(REMAP_STORAGE_KEY, JSON.stringify(inputBindings));
    } catch (err) {
      console.warn('could not save input bindings', err);
    }
  }

  function normalizeButtons(buttons) {
    const out = [];
    if (!buttons) return out;
    for (const button of buttons) {
      const b = button | 0;
      if (b >= 0 && b < buttonHolders.length && out.indexOf(b) < 0) out.push(b);
    }
    return out;
  }

  function sameButtons(a, b) {
    if (!a || !b || a.length !== b.length) return false;
    for (let i = 0; i < a.length; i++) if (a[i] !== b[i]) return false;
    return true;
  }

  function setSourceButtons(source, buttons, down) {
    const oldButtons = sourceButtons.get(source);

    if (down) {
      const normalized = normalizeButtons(buttons);
      if (!normalized.length) return false;
      if (oldButtons && sameButtons(oldButtons, normalized)) return false;
      if (oldButtons) setSourceButtons(source, null, false);
      sourceButtons.set(source, normalized);
      for (const button of normalized) {
        const holders = buttonHolders[button];
        const wasReleased = holders.size === 0;
        holders.add(source);
        if (wasReleased) wasmInputEvent(button, true);
      }
      return true;
    }

    if (!oldButtons) return false;
    sourceButtons.delete(source);
    for (const button of oldButtons) {
      const holders = buttonHolders[button];
      holders.delete(source);
      if (holders.size === 0) wasmInputEvent(button, false);
    }
    return true;
  }

  function clearInputSources() {
    sourceButtons.clear();
    gamepadSourceSet.clear();
    for (const holders of buttonHolders) holders.clear();
    wasmInputClear();
    if (DEBUG_INPUT) setStatus('Input: none');
  }

  function syncInputToWasm() {
    wasmInputClear();
    for (let button = 0; button < buttonHolders.length; button++) {
      if (buttonHolders[button].size > 0) wasmInputEvent(button, true);
    }
  }

  function describeInputState() {
    const names = [];
    for (const [name, button] of buttonNamesForDebug) {
      if (buttonHolders[button] && buttonHolders[button].size > 0) names.push(name);
    }
    return names.join('+') || 'none';
  }

  function updateDebugInputStatus() {
    if (DEBUG_INPUT) setStatus(`Input: ${describeInputState()}`);
  }

  function buttonsForToken(type, token) {
    const out = [];
    const table = inputBindings[type] || {};
    for (const action of GAME_ACTIONS) {
      const tokens = table[action.id] || [];
      if (tokens.indexOf(token) >= 0 && out.indexOf(action.button) < 0) out.push(action.button);
    }
    return out;
  }

  function removeTokenFromBindings(type, token) {
    for (const action of GAME_ACTIONS) {
      const list = inputBindings[type][action.id] || [];
      inputBindings[type][action.id] = list.filter((item) => item !== token);
    }
  }

  function clearCaptureTimeout() {
    if (captureTimeoutId) {
      clearTimeout(captureTimeoutId);
      captureTimeoutId = 0;
    }
  }

  function updateCaptureUI(message) {
    const box = document.getElementById('capture-box');
    const node = document.getElementById('capture-status');
    if (!box || !node) return;
    if (captureBinding) {
      box.hidden = false;
      node.textContent = message || `Waiting for ${captureBinding.type === 'keyboard' ? 'keyboard' : 'gamepad'} input for ${ACTION_BY_ID[captureBinding.actionId].label}. Press Escape or Cancel to abort.`;
    } else {
      box.hidden = true;
      node.textContent = '';
    }
  }

  function cancelBindingCapture(message) {
    if (!captureBinding) return;
    captureBinding = null;
    captureGamepadIgnore = new Set();
    clearCaptureTimeout();
    clearInputSources();
    updateCaptureUI('');
    renderBindingUI();
    setStatus(message || 'Control remap cancelled.');
    if (canvas) canvas.focus();
  }

  function finishBindingCapture() {
    captureBinding = null;
    captureGamepadIgnore = new Set();
    clearCaptureTimeout();
    updateCaptureUI('');
  }

  function assignBinding(type, actionId, token) {
    if (!ACTION_BY_ID[actionId] || (type !== 'keyboard' && type !== 'gamepad')) return;
    removeTokenFromBindings(type, token);
    inputBindings[type][actionId] = [token];
    saveInputBindings();
    finishBindingCapture();
    clearInputSources();
    renderBindingUI();
    setStatus(`${type === 'keyboard' ? 'Keyboard' : 'Gamepad'} ${tokenLabel(token)} mapped to ${ACTION_BY_ID[actionId].label}.`);
    if (canvas) canvas.focus();
  }

  function clearBinding(type, actionId) {
    cancelBindingCapture('Control remap cancelled.');
    if (!ACTION_BY_ID[actionId] || (type !== 'keyboard' && type !== 'gamepad')) return;
    inputBindings[type][actionId] = [];
    saveInputBindings();
    clearInputSources();
    renderBindingUI();
  }

  function resetBindings() {
    cancelBindingCapture('Control remap cancelled.');
    inputBindings = cloneBindingsFrom(DEFAULT_BINDINGS);
    saveInputBindings();
    clearInputSources();
    renderBindingUI();
    setStatus('Controls reset to defaults.');
  }

  function keyboardEventToken(event) {
    if (!event) return 'keyboard:unknown';
    if (event.key) {
      const key = event.key.length === 1 ? event.key.toLowerCase() : event.key;
      return `keyboard:key:${key}`;
    }
    if (event.code) return `keyboard:code:${event.code}`;
    if (event.keyCode || event.which) return `keyboard:keyCode:${event.keyCode || event.which}`;
    return 'keyboard:unknown';
  }

  function isKeyboardCancelEvent(event, token) {
    return token === 'keyboard:key:Escape' || token === 'keyboard:key:Esc' || event.code === 'Escape' || event.keyCode === 27;
  }

  function tokenLabel(token) {
    if (!token) return '—';
    if (token.startsWith('keyboard:key:')) {
      const key = token.slice('keyboard:key:'.length);
      if (key === ' ') return 'Space';
      return key.length === 1 ? key.toUpperCase() : key;
    }
    if (token.startsWith('keyboard:code:')) return token.slice('keyboard:code:'.length);
    if (token.startsWith('keyboard:keyCode:')) return `keyCode ${token.slice('keyboard:keyCode:'.length)}`;
    if (token.startsWith('gamepad:button:')) return `Button ${token.slice('gamepad:button:'.length)}`;
    if (token.startsWith('gamepad:axis:')) {
      const [, , axis, dir] = token.split(':');
      return `Axis ${axis} ${dir === '-' ? '−' : '+'}`;
    }
    return token;
  }

  function tokensLabel(tokens) {
    if (!tokens || !tokens.length) return '—';
    return tokens.map(tokenLabel).join(' / ');
  }

  function renderBindingUI() {
    const tbody = document.getElementById('binding-rows');
    if (!tbody) return;
    tbody.textContent = '';

    for (const action of GAME_ACTIONS) {
      const row = document.createElement('tr');
      const actionCell = document.createElement('th');
      actionCell.scope = 'row';
      actionCell.textContent = action.label;
      row.appendChild(actionCell);

      for (const type of ['keyboard', 'gamepad']) {
        const bindingCell = document.createElement('td');
        bindingCell.textContent = tokensLabel(inputBindings[type][action.id]);
        row.appendChild(bindingCell);

        const controlCell = document.createElement('td');
        const bindButton = document.createElement('button');
        bindButton.type = 'button';
        bindButton.className = 'small-button';
        bindButton.textContent = type === 'keyboard' ? 'Set key' : 'Set pad';
        bindButton.addEventListener('click', () => beginBindingCapture(type, action.id));
        controlCell.appendChild(bindButton);

        const clearButton = document.createElement('button');
        clearButton.type = 'button';
        clearButton.className = 'small-button';
        clearButton.textContent = 'Clear';
        clearButton.addEventListener('click', () => clearBinding(type, action.id));
        controlCell.appendChild(clearButton);
        row.appendChild(controlCell);
      }
      tbody.appendChild(row);
    }
  }

  function currentGamepadTokens(threshold) {
    const out = new Set();
    const getGamepads = navigator.getGamepads || navigator.webkitGetGamepads;
    if (!getGamepads) return out;
    const pads = getGamepads.call(navigator) || [];
    for (const gp of pads) {
      if (!gp) continue;
      for (let i = 0; gp.buttons && i < gp.buttons.length; i++) {
        if (gp.buttons[i] && gp.buttons[i].pressed) out.add(`gamepad:button:${i}`);
      }
      for (let i = 0; gp.axes && i < gp.axes.length; i++) {
        const v = Number(gp.axes[i]) || 0;
        if (v <= -threshold) out.add(`gamepad:axis:${i}:-`);
        else if (v >= threshold) out.add(`gamepad:axis:${i}:+`);
      }
    }
    return out;
  }

  function beginBindingCapture(type, actionId) {
    if (!ACTION_BY_ID[actionId]) return;
    if (captureBinding) cancelBindingCapture('Previous remap cancelled.');
    clearInputSources();
    captureBinding = { type, actionId, startedAt: performance.now() };
    clearCaptureTimeout();
    captureTimeoutId = setTimeout(() => {
      cancelBindingCapture('Control remap timed out.');
    }, 15000);

    const message = type === 'gamepad'
      ? `Press a gamepad button or move an axis for ${ACTION_BY_ID[actionId].label}. Press Escape or Cancel to abort.`
      : `Press a keyboard key for ${ACTION_BY_ID[actionId].label}. Press Escape or Cancel to abort.`;

    if (type === 'gamepad') {
      captureGamepadIgnore = currentGamepadTokens(0.65);
    } else {
      captureGamepadIgnore = new Set();
    }
    updateCaptureUI(message);
    renderBindingUI();
    setStatus(message);
  }

  function installControlPanel() {
    renderBindingUI();
    const reset = document.getElementById('reset-bindings');
    if (reset) reset.addEventListener('click', resetBindings);
    const cancel = document.getElementById('cancel-capture');
    if (cancel) cancel.addEventListener('click', () => cancelBindingCapture('Control remap cancelled.'));
    const close = document.getElementById('close-controls');
    if (close && controls) close.addEventListener('click', () => {
      cancelBindingCapture('Control remap cancelled.');
      controls.open = false;
      canvas.focus();
    });
    if (controls) {
      controls.addEventListener('toggle', () => {
        if (!controls.open && captureBinding) cancelBindingCapture('Control remap cancelled.');
      });
    }
  }

  function installKeyboard() {
    if (canvas) {
      canvas.tabIndex = 0;
      canvas.setAttribute('aria-label', 'Ganbare Natsuki San game canvas');
      canvas.addEventListener('contextmenu', (event) => event.preventDefault());
    }

    function setKey(event, down) {
      const token = keyboardEventToken(event);

      if (suppressedKeyboardTokens.has(token)) {
        event.preventDefault();
        event.stopPropagation();
        if (!down) suppressedKeyboardTokens.delete(token);
        return;
      }

      if (captureBinding) {
        event.preventDefault();
        event.stopPropagation();

        if (!down) return;
        if (event.repeat) return;

        suppressedKeyboardTokens.add(token);
        if (isKeyboardCancelEvent(event, token)) {
          cancelBindingCapture('Control remap cancelled.');
          return;
        }

        if (captureBinding.type === 'keyboard') {
          assignBinding('keyboard', captureBinding.actionId, token);
        }
        return;
      }

      if (controls && controls.open && controls.contains(event.target)) {
        return;
      }

      const buttons = buttonsForToken('keyboard', token);
      if (!buttons.length) return;

      if (down && event.repeat) {
        event.preventDefault();
        event.stopPropagation();
        return;
      }

      const source = `keyboard:${token}`;
      const changed = setSourceButtons(source, buttons, !!down);
      if (down) audioHost.unlock();
      if (changed) updateDebugInputStatus();

      event.preventDefault();
      event.stopPropagation();
    }

    window.addEventListener('keydown', (event) => setKey(event, true), { passive: false, capture: true });
    window.addEventListener('keyup', (event) => setKey(event, false), { passive: false, capture: true });
    window.addEventListener('blur', clearInputSources);
    document.addEventListener('visibilitychange', () => {
      if (document.hidden) clearInputSources();
    });
    canvas.addEventListener('pointerdown', () => {
      canvas.focus();
      audioHost.unlock();
    }, { passive: true });
  }

  function installPointerControls() {
    const nodes = document.querySelectorAll('[data-gns-button]');
    for (const node of nodes) {
      const name = String(node.getAttribute('data-gns-button') || '').toUpperCase();
      const button = GP2X_BUTTON_NAME[name];
      if (button === undefined) continue;

      const sourceFor = (event) => `pointer:${event.pointerId}:${name}`;
      const press = (event) => {
        event.preventDefault();
        audioHost.unlock();
        if (node.setPointerCapture) {
          try { node.setPointerCapture(event.pointerId); } catch (_) {}
        }
        setSourceButtons(sourceFor(event), [button], true);
        updateDebugInputStatus();
      };
      const release = (event) => {
        event.preventDefault();
        setSourceButtons(sourceFor(event), [button], false);
        updateDebugInputStatus();
      };

      node.addEventListener('pointerdown', press, { passive: false });
      node.addEventListener('pointerup', release, { passive: false });
      node.addEventListener('pointercancel', release, { passive: false });
      node.addEventListener('lostpointercapture', release, { passive: false });
    }
  }

  function gamepadTokensForPad(gp, threshold) {
    const tokens = [];
    if (!gp) return tokens;
    for (let i = 0; gp.buttons && i < gp.buttons.length; i++) {
      if (gp.buttons[i] && gp.buttons[i].pressed) tokens.push(`gamepad:button:${i}`);
    }
    for (let i = 0; gp.axes && i < gp.axes.length; i++) {
      const v = Number(gp.axes[i]) || 0;
      if (v <= -threshold) tokens.push(`gamepad:axis:${i}:-`);
      else if (v >= threshold) tokens.push(`gamepad:axis:${i}:+`);
    }
    return tokens;
  }

  function releaseGamepadSources() {
    let changed = false;
    for (const source of gamepadSourceSet) {
      changed = setSourceButtons(source, null, false) || changed;
    }
    gamepadSourceSet.clear();
    if (changed) updateDebugInputStatus();
  }

  function pollGamepads() {
    const getGamepads = navigator.getGamepads || navigator.webkitGetGamepads;
    if (!getGamepads) return;
    const pads = getGamepads.call(navigator) || [];

    if (controls && controls.open && !(captureBinding && captureBinding.type === 'gamepad')) {
      releaseGamepadSources();
      return;
    }

    const nextSources = new Map();

    for (const gp of pads) {
      if (!gp) continue;

      if (captureBinding && captureBinding.type === 'gamepad') {
        for (const token of gamepadTokensForPad(gp, 0.65)) {
          if (!captureGamepadIgnore.has(token)) {
            assignBinding('gamepad', captureBinding.actionId, token);
            return;
          }
        }
        continue;
      }

      for (const token of gamepadTokensForPad(gp, 0.35)) {
        const buttons = buttonsForToken('gamepad', token);
        if (!buttons.length) continue;
        nextSources.set(`gamepad:${gp.index}:${token}`, buttons);
      }
    }

    let changed = false;
    for (const source of gamepadSourceSet) {
      if (!nextSources.has(source)) changed = setSourceButtons(source, null, false) || changed;
    }
    gamepadSourceSet.clear();

    for (const [source, buttons] of nextSources) {
      changed = setSourceButtons(source, buttons, true) || changed;
      gamepadSourceSet.add(source);
    }

    if (changed) {
      audioHost.unlock();
      updateDebugInputStatus();
    }
  }

  window.addEventListener('gamepadconnected', (event) => {
    const gp = event.gamepad;
    setStatus(gp && gp.id ? `Gamepad connected: ${gp.id}` : 'Gamepad connected.');
  });

  window.addEventListener('gamepaddisconnected', () => {
    clearInputSources();
    setStatus('Gamepad disconnected.');
  });

  const imports = {
    env: {
      gns_canvas_init(w, h) {
        canvas.width = w;
        canvas.height = h;
        canvas.style.setProperty('--gns-aspect', `${w} / ${h}`);
        ctx = canvas.getContext('2d', { alpha: false });
        ctx.imageSmoothingEnabled = false;
        imageData = ctx.createImageData(w, h);
      },
      gns_canvas_present(ptr, w, h) {
        if (!ctx || !imageData) return;
        const len = w * h * 4;
        imageData.data.set(u8().subarray(ptr, ptr + len));
        ctx.putImageData(imageData, 0, 0);
      },
      gns_now_ms() {
        return performance.now();
      },
      gns_asset_size(pathPtr) {
        const entry = assetEntry(pathPtr);
        return entry ? entry.size : -1;
      },
      gns_asset_read(pathPtr, dst, capacity) {
        const entry = assetEntry(pathPtr);
        if (!entry || capacity > entry.size) return -1;
        u8().set(assetBytes.subarray(entry.offset, entry.offset + capacity), dst);
        return capacity;
      },
      gns_local_storage_load(keyPtr, dst, size) {
        const value = localStorage.getItem(localStorageKey(keyPtr));
        if (value === null) return -1;
        try {
          const bytes = bytesFromB64(value);
          if (bytes.length !== size) return -1;
          u8().set(bytes, dst);
          return 0;
        } catch (_) {
          return -1;
        }
      },
      gns_local_storage_save(keyPtr, src, size) {
        try {
          const bytes = u8().slice(src, src + size);
          localStorage.setItem(localStorageKey(keyPtr), b64FromBytes(bytes));
          return 0;
        } catch (err) {
          console.warn('could not persist save data', err);
          return -1;
        }
      },
      gns_audio_init() { return audioHost.init(); },
      gns_audio_shutdown() { audioHost.shutdown(); },
      gns_audio_load_bgm(num, pathPtr, loop) { return audioHost.loadBgm(num, pathPtr, loop); },
      gns_audio_load_se(num, pathPtr) { return audioHost.loadSe(num, pathPtr); },
      gns_audio_play_bgm(num) { return audioHost.playBgm(num); },
      gns_audio_stop_bgm() { audioHost.stopBgm(); },
      gns_audio_is_bgm_playing() { return audioHost.isBgmPlaying(); },
      gns_audio_set_bgm_gain(vol, master) { audioHost.setBgmGain(vol, master); },
      gns_audio_set_se_gain(vol, master) { audioHost.setSeGain(vol, master); },
      gns_audio_play_se(num) { return audioHost.playSe(num); },
      gns_audio_stop_se(num) { audioHost.stopSe(num); },
      gns_audio_stop_se_all() { audioHost.stopSeAll(); },
      gns_audio_is_se_playing(num) { return audioHost.isSePlaying(num); },
      gns_audio_pause() { audioHost.pause(); },
      gns_audio_resume() { audioHost.resume(); },
      gns_host_log(textPtr) {
        console.log(cstr(textPtr));
      },
      gns_host_exit(code) {
        running = false;
        setStatus(code ? `Exited with code ${code}` : '');
      }
    }
  };

  async function loadBinary(url) {
    const response = await fetch(url);
    if (!response.ok) throw new Error(`${url}: ${response.status}`);
    return response.arrayBuffer();
  }

  async function main() {
    installControlPanel();
    installKeyboard();
    installPointerControls();
    setStatus('Loading assets…');

    const [manifest, packedAssets, wasmBytes] = await Promise.all([
      fetch('assets.json').then((r) => {
        if (!r.ok) throw new Error(`assets.json: ${r.status}`);
        return r.json();
      }),
      loadBinary('assets.bin'),
      loadBinary('ganbare_natsuki_san.wasm')
    ]);

    assetManifest = manifest;
    assetBytes = new Uint8Array(packedAssets);

    setStatus('Starting…');
    const result = await WebAssembly.instantiate(wasmBytes, imports);
    instance = result.instance;
    memory = instance.exports.memory;

    const rc = instance.exports.gns_start();
    if (rc !== 0) throw new Error(`gns_start failed: ${rc}`);
    syncInputToWasm();
    canvas.focus();
    setStatus('Click or press a key to enable audio.');

    running = true;
    function frame() {
      if (!running) return;
      pollGamepads();
      const keepGoing = instance.exports.gns_frame();
      if (keepGoing) requestAnimationFrame(frame);
      else running = false;
    }
    requestAnimationFrame(frame);
  }

  main().catch((err) => {
    console.error(err);
    setStatus(String(err && err.message ? err.message : err));
  });
})();
