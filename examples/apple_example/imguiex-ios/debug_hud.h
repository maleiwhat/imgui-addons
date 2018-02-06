//
//  debug_hud.h
//  imguiex

#pragma once

typedef struct DebugHUD
{
    bool show_demo_window;
<<<<<<< HEAD
    bool show_example_window;
=======
    bool show_another_window;
>>>>>>> 0d4de754d2ffe798a79968e5b6e9b639cad0b878
    float rotation_speed;
    float cubeColor1[4];
    float cubeColor2[4];
} DebugHUD;

#if __cplusplus
extern "C" {
#endif

void DebugHUD_InitDefaults(DebugHUD *hud);
void DebugHUD_DoInterface(DebugHUD *hud);

#if __cplusplus
}
#endif
