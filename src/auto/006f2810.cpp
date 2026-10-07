#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x006F2810: stores the DWORD pointed to by `src` into the member at offset 0x1C
// of the object and returns `src` unchanged.
static uint32_t* __fastcall SetMemberAt1C(uint8_t* self, void* /*edx*/, uint32_t* src)
{
    CSF_AT(uint32_t, self, 0x1C) = *src;
    return src;
}

HOOK(0x6F2810, SetMemberAt1C, "auto");
