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

// 0x0047B140: char movement start, __thiscall (ecx = this), no stack args.
CSF_FUNC(int, __fastcall, C_0082f9f8_StartCharMovement, 0x0047B140, (C_0082f9f8* self, void*));

// 0x0048C9F0: copies a target handle into the output block, __thiscall, ret 4.
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));

// 0x004D8B80: __thiscall, ret 0.
// Marks the parent's flag word (+0x18) with bit 0, copies the parent's +0x308 pointer
// into this+0x18, starts character movement on that object, then copies the target
// handle through the parent's owner (+0x14 -> +0x204).
static int __fastcall AiAutonomous_BeginMovementAndCopyTarget(uint8_t* self, void* /*edx*/)
{
    uint8_t* parent = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));

    CSF_AT(uint32_t, parent, 0x18) |= 1u;
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, parent, 0x308);

    C_0082f9f8* movingObject = reinterpret_cast<C_0082f9f8*>(CSF_AT(uint32_t, parent, 0x308));
    C_0082f9f8_StartCharMovement(movingObject, nullptr);

    // The original reloads this+8 after the call.
    uint8_t* parentAfterCall = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));

    // 12-byte handle block on the stack: vtable, then two -1 dwords.
    alignas(8) uint8_t handleBytes[12];
    CSF_AT(void*, handleBytes, 0) = CSF_VTBL(0x0082D448);
    CSF_AT(int32_t, handleBytes, 4) = -1;
    CSF_AT(int32_t, handleBytes, 8) = -1;

    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, parentAfterCall, 0x14));
    void* targetObject = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x204));

    return AiAttackTasks_CopyTargetHandle(targetObject, nullptr, reinterpret_cast<int>(handleBytes));
}

HOOK(0x004D8B80, AiAutonomous_BeginMovementAndCopyTarget, "auto");
