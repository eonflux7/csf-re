#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// __thiscall with no stack arguments (ret 0): ecx = this, edx unused.
static int __fastcall CharActionFsm_IsFlag10000SetAndByteNonZero(void* self, void* /*edx*/)
{
    // test dword ptr [ecx + 0x2e0], 0x10000
    if ((CSF_AT(uint32_t, self, 0x2E0) & 0x10000) == 0)
        return 0;

    // mov al, byte ptr [ecx + 0x3d0]; test al, al
    if (CSF_AT(uint8_t, self, 0x3D0) == 0)
        return 0;

    return 1;
}

HOOK(0x482310, CharActionFsm_IsFlag10000SetAndByteNonZero, "auto");
