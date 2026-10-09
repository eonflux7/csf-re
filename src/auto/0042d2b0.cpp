#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sets a sprite's rotation in degrees (bound by address, cdecl; the caller pops the args).
CSF_FUNC(int, __cdecl, Sprite_SetAngleDegrees, 0x0042E460, (int a1, float a2));

// Widget vtable wrappers. Slot = byte offset / 4.
typedef int (__fastcall* WidgetRefreshFn)(void*, void*);
typedef int* (__fastcall* WidgetGetSizeFn)(void*, void*);
typedef void (__fastcall* WidgetSetPositionFn)(void*, void*, int*);

// Slot 10 (+0x28): set position from a two-int point.
static void Hud_WidgetSetPosition(void* widget, int* point) {
    ((WidgetSetPositionFn)CSF_AT(void**, widget, 0)[10])(widget, nullptr, point);
}

// Slot 2 (+0x08): refresh the widget after a change.
static int Hud_WidgetRefresh(void* widget) {
    return ((WidgetRefreshFn)CSF_AT(void**, widget, 0)[2])(widget, nullptr);
}

// Slot 15 (+0x3C): returns a pointer to the widget's {width, height} pair.
static int* Hud_WidgetGetSize(void* widget) {
    return ((WidgetGetSizeFn)CSF_AT(void**, widget, 0)[15])(widget, nullptr);
}

// Places the four corner sprites of a rectangular HUD frame. The frame rectangle is
// stored in self at +0x10 (left), +0x14 (top), +0x18 (right), +0x1C (bottom).
// Corners: top-left at 0 degrees, top-right at 90, bottom-left at 270, bottom-right at 180.
static int __fastcall Hud_PlaceCornerSprites(void* self, void* /*edx*/) {
    int point[2] = {0, 0};

    // Top-left corner: origin, 0 degrees.
    Hud_WidgetSetPosition(CSF_AT(void*, self, 0x04), point);
    Sprite_SetAngleDegrees(reinterpret_cast<int>(CSF_AT(void*, self, 0x04)), 0.0f);
    Hud_WidgetRefresh(CSF_AT(void*, self, 0x04));

    // Top-right corner: x = frame width - sprite width, y = 0, 90 degrees.
    {
        point[1] = 0;
        int frameWidth = CSF_AT(int, self, 0x18) - CSF_AT(int, self, 0x10);
        int* sprite = Hud_WidgetGetSize(CSF_AT(void*, self, 0x04));
        point[0] = frameWidth - sprite[0];
        Hud_WidgetSetPosition(CSF_AT(void*, self, 0x04), point);
        Sprite_SetAngleDegrees(reinterpret_cast<int>(CSF_AT(void*, self, 0x04)), 90.0f);
        Hud_WidgetRefresh(CSF_AT(void*, self, 0x04));
    }

    // Bottom-left corner: x = 0, y = frame height - sprite height, 270 degrees.
    {
        point[0] = 0;
        int frameHeight = CSF_AT(int, self, 0x1C) - CSF_AT(int, self, 0x14);
        int* sprite = Hud_WidgetGetSize(CSF_AT(void*, self, 0x04));
        point[1] = frameHeight - sprite[1];
        Hud_WidgetSetPosition(CSF_AT(void*, self, 0x04), point);
        Sprite_SetAngleDegrees(reinterpret_cast<int>(CSF_AT(void*, self, 0x04)), 270.0f);
        Hud_WidgetRefresh(CSF_AT(void*, self, 0x04));
    }

    // Bottom-right corner: x = frame width - sprite width, y = frame height - sprite height, 180 degrees.
    {
        int frameWidth = CSF_AT(int, self, 0x18) - CSF_AT(int, self, 0x10);
        int* spriteForX = Hud_WidgetGetSize(CSF_AT(void*, self, 0x04));
        point[0] = frameWidth - spriteForX[0];
        int frameHeight = CSF_AT(int, self, 0x1C) - CSF_AT(int, self, 0x14);
        int* spriteForY = Hud_WidgetGetSize(CSF_AT(void*, self, 0x04));
        point[1] = frameHeight - spriteForY[1];
        Hud_WidgetSetPosition(CSF_AT(void*, self, 0x04), point);
        Sprite_SetAngleDegrees(reinterpret_cast<int>(CSF_AT(void*, self, 0x04)), 180.0f);
    }

    return Hud_WidgetRefresh(CSF_AT(void*, self, 0x04));
}

HOOK(0x0042D2B0, Hud_PlaceCornerSprites, "auto");
