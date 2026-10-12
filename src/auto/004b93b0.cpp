#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Initializes a fallback query object: vtable at +0x8, sentinel -1 ids,
// zeroed counters, and a FLT_MAX float (0x7f7fffff) at +0x20.
static int* __fastcall AiCore_InitializeFallbackQuery(void* self, void* /*edx*/) {
    CSF_AT(void*, self, 0x8) = CSF_VTBL(0x0082D448);
    CSF_AT(int, self, 0xC) = -1;
    CSF_AT(int, self, 0x10) = -1;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x0) = -1;
    CSF_AT(int, self, 0x4) = -1;
    CSF_AT(int, self, 0x20) = 0x7F7FFFFF;
    CSF_AT(int, self, 0x24) = 0;
    return static_cast<int*>(self);
}

HOOK(0x004B93B0, AiCore_InitializeFallbackQuery, "auto");
