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

// Original: 0x006D2390, __thiscall, no stack args. Clears the world-link state of this object.
CSF_FUNC(void, __fastcall, S_006d2390_ClearWorldLinks, 0x006D2390, (void* self, void* edx));

// Original: 0x00690E60, __cdecl free for the game heap block.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char __fastcall S_006d2390_ReleaseBuffers(S_006d2390* self, void* /*edx*/)
{
    S_006d2390_ClearWorldLinks(self, nullptr);

    // Buffer at +0x28 (with companion fields +0x2C/+0x30).
    if (self->field_0028)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0028));
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;

    // Buffer at +0x18 (with companion fields +0x1C/+0x20).
    if (self->field_0018)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;

    // Buffer at +0x08 (with companion fields +0x0C/+0x10).
    // The result is returned in al: the low byte of Mem_FreeDebug's result
    // replaces the low byte of the original field value when the buffer was non-null.
    uint32_t releasedFlag = self->field_0008;
    if (releasedFlag)
    {
        uint32_t freeResult = static_cast<uint32_t>(Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008)));
        releasedFlag = (releasedFlag & ~0xFFu) | (freeResult & 0xFFu);
    }
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;

    return static_cast<char>(releasedFlag);
}

HOOK(0x006D2440, S_006d2390_ReleaseBuffers, "auto");
