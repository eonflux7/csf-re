#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies six dwords from the source record into self+4..self+0x18, sets the
// valid flag at self+0x20, and returns the address of the copied block (self+4).
static uint8_t* __fastcall ClumpLoader_CopySixDwordsAndMarkValid(void* self, void* /*edx*/, const uint32_t* src)
{
    CSF_AT(uint32_t, self, 0x04) = src[0];
    CSF_AT(uint32_t, self, 0x08) = src[1];
    CSF_AT(uint32_t, self, 0x0C) = src[2];
    CSF_AT(uint32_t, self, 0x10) = src[3];
    CSF_AT(uint32_t, self, 0x14) = src[4];
    CSF_AT(uint32_t, self, 0x18) = src[5];
    CSF_AT(uint8_t, self, 0x20) = 1;
    return static_cast<uint8_t*>(self) + 4;
}

HOOK(0x6FE390, ClumpLoader_CopySixDwordsAndMarkValid, "auto");
