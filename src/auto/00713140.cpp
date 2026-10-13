#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Zeroes the 64-byte (16 dword) input state block at offset 0xC50 of CInputDirectInput.
static int __fastcall CInputDirectInput_ClearStateBuffer(CInputDirectInput* self, void* /*edx*/)
{
    std::memset(&CSF_AT(uint8_t, self, 0xC50), 0, 0x40);
    return 0;
}

HOOK(0x00713140, CInputDirectInput_ClearStateBuffer, "auto");
