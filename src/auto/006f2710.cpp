#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Forwards a2 to virtual slot 2 (offset 8) of self, then returns the result of virtual slot 3 (offset 12) called with a3.
static int __fastcall ChainVirtualSetThenGet(void* self, void* /*edx*/, int a2, int a3)
{
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[2])(self, nullptr, a2);
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[3])(self, nullptr, a3);
}

HOOK(0x006F2710, ChainVirtualSetThenGet, "auto");
