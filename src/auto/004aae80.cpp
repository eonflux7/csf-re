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
#include "legacy/string.h"

// Resolves the object linked to `self` (GetLinkedObjectValue) and steps back 4 bytes;
// yields null when no link exists, as the original does (xor eax, eax).
static uint8_t* ResolveLinkedBase(void* self)
{
    CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    if (linkedValue)
        return reinterpret_cast<uint8_t*>(linkedValue - 4);
    return nullptr;
}

static int __stdcall AiMove_SetFlagStanceAndIssueCommand(int a1)
{
    CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));

    void* self = reinterpret_cast<void*>(a1);

    // Mark the linked object's flags word (+0x680) with bit 0.
    uint8_t* flagBase = ResolveLinkedBase(self);
    CSF_AT(uint32_t, flagBase, 0x680) |= 1u;

    // Set stance 2 on the object held at +0x2C of the linked base.
    uint8_t* stanceBase = ResolveLinkedBase(self);
    CoverAttack_SetStance_492720(CSF_AT(void*, stanceBase, 0x2C), nullptr, 2);

    // Issue the final virtual command (vtable slot 0x8C / 4 = 35) on the +0x2C object.
    uint8_t* commandBase = ResolveLinkedBase(self);
    void* commandObj = CSF_AT(void*, commandBase, 0x2C);
    auto commandFn = reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, commandObj, 0)[0x8C / 4]);
    return commandFn(commandObj, nullptr, 0, 0, -1, 0);
}

HOOK(0x004AAE80, AiMove_SetFlagStanceAndIssueCommand, "auto");
