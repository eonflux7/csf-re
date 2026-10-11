#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets field_003c, then forwards a2 (wrapped in an 8-byte argument block) to
// virtual slot 0x4C of the sub-object embedded at +4 of the object at field_0008.
// Original: __thiscall, one stack arg, ret 4.
static int __fastcall C_008344f0_ResetFlagAndForwardToChild(C_008344f0* self, void* /*edx*/, int a2)
{
    self->field_003c = 0;

    uint8_t* linkedObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));
    uint8_t* childObj = linkedObj + 4;

    // Stack block the original passes by address: int at +0, byte flag at +4 (zeroed).
    alignas(8) uint8_t argBlock[8] = {};
    *reinterpret_cast<int*>(argBlock) = a2;
    argBlock[4] = 0;

    return ((int(__fastcall*)(void*, void*, int*))CSF_AT(void**, childObj, 0)[0x4C / 4])(
        childObj, nullptr, reinterpret_cast<int*>(argBlock));
}

HOOK(0x4A2F90, C_008344f0_ResetFlagAndForwardToChild, "auto");
