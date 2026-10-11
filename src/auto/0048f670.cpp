#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static char __fastcall Game_SetParametersAndMarkDirty(
    void* self, void* /*edx*/, const void* parameters)
{
    void* linkedObject = CSF_AT(void*, self, 0xA8);
    CSF_AT(uint8_t, self, 0x171) = 1;
    CSF_AT(uint8_t, linkedObject, 0x0D) = 1;

    CSF_AT(uint32_t, self, 0xF0) = CSF_AT(uint32_t, parameters, 0x00);
    CSF_AT(uint32_t, self, 0xF4) = CSF_AT(uint32_t, parameters, 0x04);
    CSF_AT(uint32_t, self, 0xFC) = CSF_AT(uint32_t, parameters, 0x0C);
    CSF_AT(uint32_t, self, 0x100) = CSF_AT(uint32_t, parameters, 0x10);
    CSF_AT(uint32_t, self, 0x104) = CSF_AT(uint32_t, parameters, 0x14);
    CSF_AT(uint32_t, self, 0x108) = CSF_AT(uint32_t, parameters, 0x18);
    CSF_AT(uint32_t, self, 0x10C) = CSF_AT(uint32_t, parameters, 0x1C);
    CSF_AT(uint8_t, self, 0x110) = CSF_AT(uint8_t, parameters, 0x20);
    CSF_AT(uint8_t, self, 0x111) = CSF_AT(uint8_t, parameters, 0x21);
    char result = CSF_AT(char, parameters, 0x22);
    CSF_AT(char, self, 0x112) = result;
    return result;
}

HOOK(0x0048F670, Game_SetParametersAndMarkDirty, "auto");
