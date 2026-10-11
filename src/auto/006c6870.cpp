#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Resolves an object pointer from the raw argument value (sub_681FB0, __fastcall, returns int).
CSF_FUNC(int, __fastcall, ResolveSkinHandleFromArg, 0x00681FB0, (void* self, void* edx));

// Processes one skinned part (sub_6C63D0, __thiscall-style via __fastcall, pops 4 stack bytes).
CSF_FUNC(char, __fastcall, ProcessSkinnedPart, 0x006C63D0, (void* self, void* edx, int a2));

// Bumps the call counter at +0x104. If the flag bit 0x20 (field_0008) is set, resolves the
// argument, processes it, and on success bumps the success counter at +0x120.
static char __fastcall C_008656d0_ProcessIfBit20Set(C_008656d0* self, void* /*edx*/, void* a2)
{
    uint32_t callCounter = CSF_AT(uint32_t, self, 0x104);
    uint8_t flags = self->field_0008;
    char result = 0;

    CSF_AT(uint32_t, self, 0x104) = callCounter + 1;

    if ((flags & 0x20) != 0)
    {
        int resolved = ResolveSkinHandleFromArg(a2, nullptr);
        result = ProcessSkinnedPart(self, nullptr, resolved);
        if (result)
        {
            CSF_AT(int, self, 0x120) += 1;
        }
    }
    return result;
}

HOOK(0x006C6870, C_008656d0_ProcessIfBit20Set, "auto");
