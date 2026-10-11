#include <cstdint>
#include "globals.h"
#include "hook.h"

struct C_0082f9f8;  // forward declaration only; the class is not defined in auto_classes.h

CSF_FUNC(char, __fastcall, AttackTasks_SetFieldTransitionScale, 0x0047B150, (uint32_t* self, void* , char enableScale));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovementAndSetFlag, 0x0047E0C0, (C_0082f9f8* self, void* , int a2, uint8_t a3));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));

// Marks the owner's flag word, stores the owner's movement object into self, starts
// movement toward arg0, enables the field transition scale and copies the target
// handle of the owner's target object into a local handle record.
static int __fastcall Autonomo_BeginAttackTargetCopy(void* self, void* /*edx*/, int arg0)
{
    // *(owner + 0x18) |= 1
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x08);
    CSF_AT(uint32_t, owner, 0x18) |= 1u;

    // self->movement = owner->field_0x308
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, owner, 0x308);

    // C_0082f9f8_StartMovementAndSetFlag(movement, arg0, 0)
    C_0082f9f8_StartMovementAndSetFlag(
        reinterpret_cast<C_0082f9f8*>(static_cast<uintptr_t>(CSF_AT(uint32_t, owner, 0x308))),
        nullptr,
        arg0,
        0);

    // AttackTasks_SetFieldTransitionScale(movement, 1)
    AttackTasks_SetFieldTransitionScale(
        reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x18))),
        nullptr,
        1);

    // Build the handle record: vtable, -1, -1
    uint8_t* ownerAgain = CSF_AT(uint8_t*, self, 0x08);
    uint8_t* targetOwner = CSF_AT(uint8_t*, ownerAgain, 0x14);

    int handle[3];
    handle[0] = static_cast<int>(reinterpret_cast<intptr_t>(CSF_VTBL(0x0082D448)));
    handle[1] = -1;
    handle[2] = -1;

    // Reference to the S_0048c9f0 vtable used by the copy routine (no runtime effect here).
    (void)CSF_VTBL(0x0048C9F0);

    return AiAttackTasks_CopyTargetHandle(
        reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, targetOwner, 0x204))),
        nullptr,
        static_cast<int>(reinterpret_cast<intptr_t>(handle)));
}

HOOK(0x004D8BE0, Autonomo_BeginAttackTargetCopy, "auto");
