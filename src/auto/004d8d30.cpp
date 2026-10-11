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

// Callees (bound by address)
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovementAndAttack, 0x0047F8F0, (C_0082f9f8* self, void* /*edx*/, void* arg0, unsigned int a3, int a4));
CSF_FUNC(char, __fastcall, AttackTasks_SetFieldTransitionScale, 0x0047B150, (uint32_t* self, void* /*edx*/, char enableScale));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* /*edx*/, int a2));

// Starts the attack on the autonomous AI: marks the owner's flag bit 0, starts
// movement and attack with the three stack arguments, enables the field
// transition scale, then copies the target handle of the owner's parent task
// into a local handle record and returns the copy result.
static int __fastcall AiAutonomous_StartMoveAttackAndCopyTargetHandle(void* self, void* /*edx*/, void* arg0, unsigned int a3, int a4)
{
    // owner = self->field_0008
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));

    // *(owner + 0x18) |= 1
    CSF_AT(uint32_t, owner, 0x18) |= 1u;

    // self->field_0018 = owner->field_0308
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, owner, 0x308);

    // StartMovementAndAttack: this = owner->field_0308
    C_0082f9f8_StartMovementAndAttack(
        reinterpret_cast<C_0082f9f8*>(CSF_AT(uint32_t, owner, 0x308)),
        nullptr,
        arg0,
        a3,
        a4);

    // this = self->field_0018 (reloaded after the call)
    AttackTasks_SetFieldTransitionScale(
        reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x18)),
        nullptr,
        1);

    // Reload owner, then owner->field_0014->field_0204 is the copy source
    uint8_t* ownerAgain = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    uint8_t* parentTask = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, ownerAgain, 0x14));
    S_0048c9f0* targetHandleSource = reinterpret_cast<S_0048c9f0*>(CSF_AT(uint32_t, parentTask, 0x204));

    // Handle record: vtable/tag, then two invalid ids
    int handle[3];
    handle[0] = static_cast<int>(reinterpret_cast<intptr_t>(CSF_VTBL(0x0082D448)));
    handle[1] = -1;
    handle[2] = -1;

    return AiAttackTasks_CopyTargetHandle(targetHandleSource, nullptr, reinterpret_cast<int>(handle));
}

HOOK(0x004D8D30, AiAutonomous_StartMoveAttackAndCopyTargetHandle, "auto");
