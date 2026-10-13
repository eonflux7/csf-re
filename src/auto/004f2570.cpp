#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* , uint8_t* a2));
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall HudStringShow_InitializeEntry(C_0082d410* self, void* /*edx*/) {
    CSF_AT(uint32_t, self, 0x18) = 15;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x34) = 7;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint16_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint32_t, self, 0x54) = 0;
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x60) = 0;
    CSF_AT(uint32_t, self, 0x64) = 0;
    CSF_AT(uint32_t, self, 0x68) = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x70) = 0;

    AssignStringRange_429B60(self, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

    S_004f21d0* textInput = reinterpret_cast<S_004f21d0*>(reinterpret_cast<uint8_t*>(self) + 0x1C);
    TextInputSubobject_Init(textInput, nullptr, &g_byte_82E65C);

    CSF_AT(uint32_t, self, 0x3C) = 0xBF800000u;
    CSF_AT(uint32_t, self, 0x40) = 0xBF800000u;
    CSF_AT(uint32_t, self, 0x44) = 0xBF800000u;

    CSF_AT(uint8_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x004F2570, HudStringShow_InitializeEntry, "auto");
