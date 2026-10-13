#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

static int __fastcall DevConsole_Initialize(C_0085ec18* self, void* /*edx*/) {
    // Resolve the menu controls object from the global, then query its metric
    // (vtable slot 18) and store the result at this+0xF0.
    void* globalObj = reinterpret_cast<void*>(GetGlobalInt());
    void* menuControls = MenuControls_ResolveObject(globalObj, nullptr);
    CSF_AT(int, self, 0xF0) =
        ((int(__fastcall*)(void*, void*))(CSF_AT(void**, menuControls, 0)[18]))(menuControls, nullptr);

    // Resolve again and load the font by name (vtable slot 20); store at this+0xF4.
    globalObj = reinterpret_cast<void*>(GetGlobalInt());
    void* menuControls2 = MenuControls_ResolveObject(globalObj, nullptr);
    int font = ((int(__fastcall*)(void*, void*, const char*))(CSF_AT(void**, menuControls2, 0)[20]))(
        menuControls2, nullptr, CSF_STR(0x0083F1E4, "Menus\\Ingame_01.fnt"));
    CSF_AT(int, self, 0xF4) = font;

    // Build the 4-float rect {255, 0, 0, 255} and pass it to the font object (vtable slot 24).
    float rect[4] = {255.0f, 0.0f, 0.0f, 255.0f};
    return ((int(__fastcall*)(void*, void*, float*))(CSF_AT(void**, reinterpret_cast<void*>(font), 0)[24]))(
        reinterpret_cast<void*>(font), nullptr, rect);
}

HOOK(0x004F9670, DevConsole_Initialize, "auto");
