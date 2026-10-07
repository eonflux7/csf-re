#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// 0x006F2840 (15 bytes, __thiscall, ret 4)
// Reads the pointer-sized value stored at this+0x20 and writes it through the
// caller-supplied out parameter, returning that same out pointer.
static uint32_t* __fastcall GetMemberPointer0020(void* self, void* /*edx*/, uint32_t* outValue)
{
    *outValue = CSF_AT(uint32_t, self, 0x20);
    return outValue;
}

HOOK(0x6F2840, GetMemberPointer0020, "auto");
