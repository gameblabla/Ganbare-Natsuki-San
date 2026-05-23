#include "../../define.h"
#include "../../input/input.h"
#include "../../general.h"
#include "../../extern.h"
#include "../../function.h"
#include <stdlib.h>
#include <string.h>

typedef struct WasmCanvasInput {
    /* Raw event state owned by the browser host.  JS writes these immediately
       from keydown/keyup, pointer, and gamepad transitions. */
    int raw_press[GP2X_BUTTON_MAX];
    int event_push[GP2X_BUTTON_MAX];

    /* Stable one-frame snapshot consumed by the game.  This deliberately
       mirrors the legacy SDL backend: Input_IsPush() is true for the whole
       frame, while Input_IsPress() is true while the control is held. */
    int frame_press[GP2X_BUTTON_MAX];
    int frame_push[GP2X_BUTTON_MAX];
    int prev_press[GP2X_BUTTON_MAX];
} WasmCanvasInput;

static WasmCanvasInput *g_input;

static int valid_button(int button)
{
    return button >= 0 && button < GP2X_BUTTON_MAX;
}

void Input_Init(void)
{
    if (g_input) return;
    g_input = (WasmCanvasInput*)calloc(1, sizeof(*g_input));
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

__attribute__((export_name("gns_input_button_event")))
void gns_input_button_event(int button, int pressed)
{
    if (!g_input || !valid_button(button)) return;

    if (pressed) {
        if (!g_input->raw_press[button]) {
            /* Latch the edge immediately.  This makes very short browser key
               taps survive until the next exported gns_frame() snapshot. */
            g_input->event_push[button] = 1;
        }
        g_input->raw_press[button] = 1;
    } else {
        g_input->raw_press[button] = 0;
    }
}

__attribute__((export_name("gns_input_clear")))
void gns_input_clear(void)
{
    if (!g_input) return;
    memset(g_input, 0, sizeof(*g_input));
}

__attribute__((export_name("gns_input_debug_press")))
int gns_input_debug_press(int button)
{
    if (!g_input || !valid_button(button)) return 0;
    return g_input->frame_press[button] ? 1 : 0;
}

__attribute__((export_name("gns_input_debug_push")))
int gns_input_debug_push(int button)
{
    if (!g_input || !valid_button(button)) return 0;
    return g_input->frame_push[button] ? 1 : 0;
}

void Input_Update(void)
{
    int i;

    if (!g_input) return;

    for (i = 0; i < GP2X_BUTTON_MAX; i++) {
        g_input->frame_press[i] = g_input->raw_press[i];
        g_input->frame_push[i] =
            (g_input->event_push[i] ||
             (g_input->raw_press[i] && !g_input->prev_press[i])) ? 1 : 0;

        g_input->event_push[i] = 0;
        g_input->prev_press[i] = g_input->raw_press[i];
    }
}

int Input_IsPush(int keycode)
{
    if (!g_input || keycode < 0 || keycode >= GP2X_BUTTON_MAX) return 0;
    return g_input->frame_push[keycode] == 1;
}

int Input_IsPress(int keycode)
{
    if (!g_input || keycode < 0 || keycode >= GP2X_BUTTON_MAX) return 0;
    return g_input->frame_press[keycode] == 1;
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
