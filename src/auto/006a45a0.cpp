#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x006A3330: __fastcall, ecx = a1 (subobject base), edx = owner. Returns uint16_t*.
CSF_FUNC(uint16_t*, __fastcall, LogWindow_InitSubobjects, 0x006A3330, (int a1, int a2));

// Resets the DlgLog window state inside the owner object, then tail-calls LogWindow_InitSubobjects.
static uint16_t* __cdecl LogWindow_InitState(int a1) {
    auto* self = reinterpret_cast<uint8_t*>(a1);

    CSF_AT(uint32_t, self, 0xB10) = (uint32_t)(uintptr_t)(self + 0x8C);
    CSF_AT(uint32_t, self, 0xB28) = (uint32_t)(uintptr_t)(self + 0xA74);
    CSF_AT(uint32_t, self, 0xB18) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x008A6BBC);
    CSF_AT(uint32_t, self, 0xB1C) = (uint32_t)(uintptr_t)(self + 0x980);
    CSF_AT(uint32_t, self, 0xB24) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x008A6BD0);
    CSF_AT(uint32_t, self, 0xB30) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x008A6BE4);
    CSF_AT(uint16_t, self, 0x16B0) = 0;
    CSF_AT(int, self, 0x16B4) = 0;
    CSF_AT(int, self, 0x16AC) = 8;

    // Tail jump: forwards ecx = a1 + 0x980 and edx = a1.
    return LogWindow_InitSubobjects(a1 + 0x980, a1);
}

HOOK(0x006A45A0, LogWindow_InitState, "auto");
