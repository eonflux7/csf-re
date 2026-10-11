#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// __thiscall(this in ecx, one stack arg), ret 4.
// Copies the dword that `source` points to into this+0x14 and returns `source`.
static uint32_t* __fastcall CopyDwordIntoField0014(void* self, void* /*edx*/, uint32_t* source)
{
    CSF_AT(uint32_t, self, 0x14) = *source;
    return source;
}

HOOK(0x420C80, CopyDwordIntoField0014, "auto");
