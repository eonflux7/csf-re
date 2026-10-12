#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwarder: __fastcall, ecx = owner, no stack arguments.
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_TestCoverPosition, 0x004E5C70, (C_0083e9d0* self, void* , float* arg0, int a2, int a4));

// Tests a cover position offset vertically by the difference between the owner's
// position (vtable slot 41) and the object position of the owner. Returns the
// AiCoverAttackTasks_TestCoverPosition result.
static int __fastcall AiMoveTasks_TestCoverPositionAdjusted(void* self, void* /*edx*/, float* a2, int a3)
{
    float pos[3];
    pos[0] = a2[0];
    pos[1] = a2[1];
    pos[2] = a2[2];

    void* owner = CSF_AT(void*, self, 0x2C);

    // Out buffer for the owner's position (12 bytes on the original's stack).
    float ownerPos[3];
    float* targetPos = reinterpret_cast<float*>(
        ((void* (__fastcall*)(void*, void*, float*))CSF_AT(void**, owner, 0)[0xA4 / 4])(owner, nullptr, ownerPos));

    float* objectPos = reinterpret_cast<float*>(GetObjectPositionVector(owner, nullptr));

    pos[1] = (targetPos[1] - objectPos[1]) + pos[1];

    // The original re-reads self+0x2C here, after the calls above.
    C_0083e9d0* coverSelf = reinterpret_cast<C_0083e9d0*>(
        CSF_AT(void*, CSF_AT(void*, self, 0x2C), 0x3C));

    return AiCoverAttackTasks_TestCoverPosition(coverSelf, nullptr, pos, a3, -1);
}

HOOK(0x004AA3D0, AiMoveTasks_TestCoverPositionAdjusted, "auto");
