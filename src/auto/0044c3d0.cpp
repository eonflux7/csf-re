#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* self, void* edx, char* text, unsigned int length));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void* __fastcall C_0082d448_InitDefaults(void* self, void* /*edx*/) {
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(void*, self, 0x10) = CSF_VTBL(0x0082D448);
    CSF_AT(int32_t, self, 0x14) = -1;
    CSF_AT(int32_t, self, 0x18) = -1;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x58) = 15;
    CSF_AT(uint32_t, self, 0x54) = 0;

    // v2 = (C_0082d410*)(this + 16): sub-object at +0x40; its field_0004 is at +0x44.
    void* subObject = reinterpret_cast<uint8_t*>(self) + 0x40;
    CSF_AT(uint8_t, subObject, 0x04) = 0;

    CSF_AT(uint8_t, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x60) = 0;
    CSF_AT(int32_t, self, 0x00) = -1;
    CSF_AT(int32_t, self, 0x38) = -1;
    CSF_AT(int32_t, self, 0x3C) = -1;

    AssignStringRange_429B60(subObject, nullptr,
                             reinterpret_cast<char*>(&g_byte_82E65C), 0u);

    CSF_AT(int32_t, self, 0x20) = -1;
    CSF_AT(int32_t, self, 0x24) = -1;
    CSF_AT(int32_t, self, 0x1C) = -1;
    CSF_AT(uint8_t, self, 0x28) = 0;
    return self;
}

HOOK(0x0044C3D0, C_0082d448_InitDefaults, "auto");
