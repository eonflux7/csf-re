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

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));

// Resolves the linked object for `self` to the base pointer used by the original (value - 4), or null.
static uint8_t* ResolveLinkedBase(void* self)
{
    int linked = GetLinkedObjectValue_4DB5C0(self, nullptr);
    if (linked)
        return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linked - 4));
    return nullptr;
}

static int __stdcall TMovilAPunto_ResetAndSetStance(int a1)
{
    // Clear the counter at +0x6DC on the first linked base.
    uint8_t* firstBase = ResolveLinkedBase(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)));
    CSF_AT(int, firstBase, 0x6dc) = 0;

    // Second lookup: its linked base's sub-object at +0x2C receives stance 3.
    uint8_t* secondBase = ResolveLinkedBase(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)));
    void* stanceObject = secondBase ? CSF_AT(void*, secondBase, 0x2c) : nullptr;
    CoverAttack_SetStance_492720(stanceObject, nullptr, 3);

    // Third lookup: call virtual slot 0x8C/4 = 35 on the sub-object at +0x2C.
    uint8_t* thirdBase = ResolveLinkedBase(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)));
    void* targetObject = CSF_AT(void*, thirdBase, 0x2c);
    return ((int(__fastcall*)(void*, void*, int, int, int, int))
            CSF_AT(void**, targetObject, 0)[0x8c / 4])(targetObject, nullptr, 0, 0, -1, 0);
}

HOOK(0x004AB7A0, TMovilAPunto_ResetAndSetStance, "auto");
