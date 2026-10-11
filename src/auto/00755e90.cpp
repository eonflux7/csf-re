#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Resets the object's two string buffers to empty and sets the default sentinel values.
static C_0082d410* __fastcall C_0082d410_InitDefaults(C_0082d410* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 15;
    CSF_AT(uint8_t, self, 0x08) = 0;
    CSF_AT(int, self, 0x20) = 0;
    CSF_AT(int, self, 0x48) = 15;
    CSF_AT(int, self, 0x44) = 0;
    CSF_AT(uint8_t, self, 0x34) = 0;
    CSF_AT(int, self, 0x00) = -1;

    AssignStringRange_429B60((uint8_t*)self + 4, nullptr,
                             reinterpret_cast<char*>(&g_byte_82E65C), 0);

    CSF_AT(int, self, 0x20) = 255;
    CSF_AT(int, self, 0x24) = -1;
    CSF_AT(int, self, 0x28) = -1;
    CSF_AT(int, self, 0x2C) = -1;

    AssignStringRange_429B60((uint8_t*)self + 0x30, nullptr,
                             reinterpret_cast<char*>(&g_byte_82E65C), 0);

    CSF_AT(int, self, 0x4C) = -1;
    return self;
}

HOOK(0x00755E90, C_0082d410_InitDefaults, "auto");
