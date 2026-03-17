#ifndef INPUT_H
#define INPUT_H

void Input_Init(void);
void Input_Shutdown(void);
void Input_Update(void);
int Input_PollEvent(void);
int Input_IsPush(int keycode);
int Input_IsPress(int keycode);
int Input_IsPushOK(void);
int Input_IsPushCancel(void);
int Input_SystemKeys(void);

#endif
