#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));

// Scans the handle list held by the action FSM (field_0078 .. field_007c) and
// returns the index of the first handle whose value equals `value`, or -1.
static int __fastcall CCharActionFsm_FindHandleValueIndex(CCharActionFsm* self, void* /*edx*/, int value)
{
    uint32_t index = 0;
    for (;; ++index) {
        uint32_t listBegin = self->field_0078;
        if (listBegin == 0)
            break;

        int32_t listCount = static_cast<int32_t>(self->field_007c - listBegin) >> 2;
        if (index >= static_cast<uint32_t>(listCount))
            break;

        uint32_t handle = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(listBegin) + 4 * index);
        int valueAddress = Handle_GetValuePointer(reinterpret_cast<void*>(static_cast<uintptr_t>(handle)), nullptr);
        int* valuePointer = reinterpret_cast<int*>(static_cast<uintptr_t>(static_cast<uint32_t>(valueAddress)));

        if (*valuePointer == value)
            return static_cast<int>(index);
    }
    return -1;
}

HOOK(0x004859D0, CCharActionFsm_FindHandleValueIndex, "auto");
