#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: CoverAttack_CheckState_4ADAE0, bound as in the packet.
CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));

// Returns the result of CoverAttack_CheckState_4ADAE0 for one of two embedded
// sub-objects of this: the one at +0x4C normally, or the one at +0x34 when
// field_0054 is negative (signed compare, as in the original's jge).
static char __fastcall AiSearch_HasAttackTarget(S_004ad990* self, void* /*edx*/)
{
    uint8_t* subObject = (uint8_t*)self + 0x4c;
    if (static_cast<int32_t>(self->field_0054) < 0)
        subObject = (uint8_t*)self + 0x34;

    return CoverAttack_CheckState_4ADAE0(self, nullptr, (int)subObject);
}

HOOK(0x004B22B0, AiSearch_HasAttackTarget, "auto");
