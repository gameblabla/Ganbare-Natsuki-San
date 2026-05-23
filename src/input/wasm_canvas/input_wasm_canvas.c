#include "../../define.h"
#include "../../input/input.h"
#include "../../general.h"
#include "../../extern.h"
#include "../../function.h"
#include <stdlib.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

EM_JS(void, gns_input_init_js, (void), {
    if (Module['gnsInputInstalled']) return;
    Module['gnsInputInstalled'] = true;
    Module['gnsKeys'] = Object.create(null);

    function setKey(event, down) {
        Module['gnsKeys'][event.code] = down ? 1 : 0;
        if (event.code === 'ArrowUp' || event.code === 'ArrowDown' ||
            event.code === 'ArrowLeft' || event.code === 'ArrowRight' ||
            event.code === 'Space' || event.code === 'ControlLeft' ||
            event.code === 'AltLeft' || event.code === 'F1' || event.code === 'F2') {
            event.preventDefault();
        }
    }

    window.addEventListener('keydown', function(event) { setKey(event, 1); }, { passive: false });
    window.addEventListener('keyup', function(event) { setKey(event, 0); }, { passive: false });
    window.addEventListener('blur', function() { Module['gnsKeys'] = Object.create(null); });
});

EM_JS(int, gns_key_down_js, (int key), {
    var k = Module['gnsKeys'] || Object.create(null);
    switch (key) {
        case 1: return k['ArrowRight'] || k['Numpad6'] ? 1 : 0;
        case 2: return k['ArrowLeft']  || k['Numpad4'] ? 1 : 0;
        case 3: return k['ArrowDown']  || k['Numpad2'] ? 1 : 0;
        case 4: return k['ArrowUp']    || k['Numpad8'] ? 1 : 0;
        case 5: return k['KeyD'] || k['Numpad6'] ? 1 : 0;
        case 6: return k['KeyA'] || k['Numpad4'] ? 1 : 0;
        case 7: return k['KeyS'] || k['Numpad2'] ? 1 : 0;
        case 8: return k['KeyW'] || k['Numpad8'] ? 1 : 0;
        case 9: return k['ControlLeft'] || k['KeyX'] ? 1 : 0;
        case 10: return k['AltLeft'] || k['Space'] || k['KeyC'] ? 1 : 0;
        case 11: return k['Enter'] ? 1 : 0;
        case 12: return k['Backslash'] ? 1 : 0;
        case 13: return k['ShiftRight'] ? 1 : 0;
        case 14: return k['KeyP'] ? 1 : 0;
        case 15: return k['F1'] ? 1 : 0;
        case 16: return k['F2'] ? 1 : 0;
        case 17: return k['Escape'] ? 1 : 0;
        default: return 0;
    }
});
#else
static void gns_input_init_js(void) {}
static int gns_key_down_js(int key) { (void)key; return 0; }
#endif

typedef struct WasmCanvasInput {
    int key_eventPress[GP2X_BUTTON_MAX];
    int key_eventPress_old[GP2X_BUTTON_MAX];
    int key_eventPush[GP2X_BUTTON_MAX];
    int pad_type;
} WasmCanvasInput;

static WasmCanvasInput *g_input;

void Input_Init(void)
{
    if (g_input) return;
    g_input = (WasmCanvasInput*)calloc(1, sizeof(*g_input));
    gns_input_init_js();
}

void Input_Shutdown(void)
{
    free(g_input);
    g_input = NULL;
}

int Input_PollEvent(void)
{
    return 0;
}

void Input_Update(void)
{
    int pad = 0;
    int i;

    if (!g_input) return;

    if (g_input->pad_type == 0) {
        if (gns_key_down_js(1)) pad |= PAD_RIGHT;
        if (gns_key_down_js(2)) pad |= PAD_LEFT;
        if (gns_key_down_js(3)) pad |= PAD_DOWN;
        if (gns_key_down_js(4)) pad |= PAD_UP;
        if (gns_key_down_js(9)) pad |= PAD_BUTTON1;
        if (gns_key_down_js(10)) pad |= PAD_BUTTON2;
        if (gns_key_down_js(11)) pad |= PAD_BUTTON3;
    } else {
        if (gns_key_down_js(5)) pad |= PAD_RIGHT;
        if (gns_key_down_js(6)) pad |= PAD_LEFT;
        if (gns_key_down_js(7)) pad |= PAD_DOWN;
        if (gns_key_down_js(8)) pad |= PAD_UP;
        if (gns_key_down_js(12)) pad |= PAD_BUTTON1;
        if (gns_key_down_js(13)) pad |= PAD_BUTTON2;
        if (gns_key_down_js(14)) pad |= PAD_BUTTON3;
    }

    if (gns_key_down_js(15)) pad |= PAD_BUTTON7;
    if (gns_key_down_js(16)) pad |= PAD_BUTTON8;

    for (i = 0; i < GP2X_BUTTON_MAX; i++) g_input->key_eventPress[i] = 0;

    if (pad & PAD_UP) g_input->key_eventPress[GP2X_BUTTON_UP] = 1;
    if (pad & PAD_DOWN) g_input->key_eventPress[GP2X_BUTTON_DOWN] = 1;
    if (pad & PAD_LEFT) g_input->key_eventPress[GP2X_BUTTON_LEFT] = 1;
    if (pad & PAD_RIGHT) g_input->key_eventPress[GP2X_BUTTON_RIGHT] = 1;
    if (pad & PAD_BUTTON1) g_input->key_eventPress[GP2X_BUTTON_A] = 1;
    if (pad & PAD_BUTTON2) g_input->key_eventPress[GP2X_BUTTON_X] = 1;
    if (pad & PAD_BUTTON3) g_input->key_eventPress[GP2X_BUTTON_Y] = 1;
    if (pad & PAD_BUTTON4) g_input->key_eventPress[GP2X_BUTTON_B] = 1;
    if (pad & PAD_BUTTON5) g_input->key_eventPress[GP2X_BUTTON_R] = 1;
    if (pad & PAD_BUTTON6) g_input->key_eventPress[GP2X_BUTTON_L] = 1;
    if (pad & PAD_BUTTON7) g_input->key_eventPress[GP2X_BUTTON_VOLDOWN] = 1;
    if (pad & PAD_BUTTON8) g_input->key_eventPress[GP2X_BUTTON_VOLUP] = 1;
    if (pad & PAD_BUTTON9) g_input->key_eventPress[GP2X_BUTTON_SELECT] = 1;
    if (pad & PAD_BUTTONA) g_input->key_eventPress[GP2X_BUTTON_START] = 1;
    if (pad & PAD_BUTTONB) g_input->key_eventPress[GP2X_BUTTON_CLICK] = 1;
    if (gns_key_down_js(17)) g_input->key_eventPress[GP2X_BUTTON_EXIT] = 1;

    for (i = 0; i < GP2X_BUTTON_MAX; i++) {
        g_input->key_eventPush[i] = (!g_input->key_eventPress_old[i] && g_input->key_eventPress[i]) ? 1 : 0;
        g_input->key_eventPress_old[i] = g_input->key_eventPress[i];
    }
}

int Input_IsPush(int keycode)
{
    if (!g_input || keycode < 0 || keycode >= GP2X_BUTTON_MAX) return 0;
    return g_input->key_eventPush[keycode] == 1;
}

int Input_IsPress(int keycode)
{
    if (!g_input || keycode < 0 || keycode >= GP2X_BUTTON_MAX) return 0;
    return g_input->key_eventPress[keycode] == 1;
}

int Input_IsPushOK(void)
{
    return Input_IsPush(GP2X_BUTTON_A);
}

int Input_IsPushCancel(void)
{
    return Input_IsPush(GP2X_BUTTON_X);
}

int Input_SystemKeys(void)
{
    if (Input_IsPress(GP2X_BUTTON_EXIT)) {
        g_scene = EN_SN_EXIT;
        return 0;
    }

    if (Input_IsPush(GP2X_BUTTON_VOLUP)) {
        gameflag[60] += 10;
        if (gameflag[60] > VOL_MAX) gameflag[60] = VOL_MAX;
        Set_Volume(gameflag[60]);
    }

    if (Input_IsPush(GP2X_BUTTON_VOLDOWN)) {
        gameflag[60] -= 10;
        if (gameflag[60] < 0) gameflag[60] = 0;
        Set_Volume(gameflag[60]);
    }

    return 1;
}
