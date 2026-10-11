#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, PrepareCoverAttackTarget, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, S_004c6f80_ExecuteCoverAttack, 0x004C6F80, (void* self, void* edx, int a2, char a3));

static void* AddressFromInt(int address)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

static int __fastcall S_004c6f80_TryAttackFromCoverPosition(S_004c6f80* self, void* /*edx*/, int a2, int a3)
{
    int returnValue;

    PrepareCoverAttackTarget(a2, a3);
    returnValue = IsField10Null(reinterpret_cast<void*>(a2), nullptr);
    if (static_cast<uint8_t>(returnValue) == 0 && self->field_0004 != 0)
    {
        int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a2), nullptr);
        returnValue = linked ? linked - 4 : 0;
        if (CSF_AT(int, AddressFromInt(returnValue), 0x608) != -1)
        {
            linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a2), nullptr);
            returnValue = linked ? linked - 4 : 0;
            if (CSF_AT(uint8_t, AddressFromInt(returnValue), 0x684) != 0)
            {
                returnValue = S_004c6f80_ExecuteCoverAttack(self, nullptr, a2, 0);
            }
        }
    }
    return returnValue;
}

HOOK(0x004C71F0, S_004c6f80_TryAttackFromCoverPosition, "auto");
