#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));

// Starts an autonomous task: marks the owner context busy, stores the task object,
// forwards (arg0, a3) to its virtual slot 43, then copies the target handle
// into a temporary record and clears the byte at +0x2EC of the attached object.
static int __fastcall TAutonomo_StartTaskAndCopyTargetHandle(void* self, void* /*edx*/, int arg0, int a3, char a4)
{
    void* ownerCtx = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    CSF_AT(uint32_t, ownerCtx, 0x18) |= 1u;

    void* aiData = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerCtx, 0x14));
    void* taskObj = reinterpret_cast<void*>(CSF_AT(uint32_t, aiData, 0x30));
    CSF_AT(uint32_t, self, 0x14) = reinterpret_cast<uint32_t>(taskObj);
    CSF_AT(int, taskObj, 0x1C) = (a4 != 0) ? 1 : 0;

    reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(CSF_AT(void**, taskObj, 0)[43])(
        taskObj, nullptr, arg0, a3);

    int a2[3];
    a2[1] = -1;
    a2[2] = -1;
    a2[0] = static_cast<int>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));

    void* ownerCtxAgain = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    void* aiDataAgain = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerCtxAgain, 0x14));
    void* targetHandle = reinterpret_cast<void*>(CSF_AT(uint32_t, aiDataAgain, 0x204));
    int result = AiAttackTasks_CopyTargetHandle(
        targetHandle, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(a2)));

    void* attachedObj = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x18));
    if (attachedObj) {
        CSF_AT(uint8_t, attachedObj, 0x2EC) = 0;
    }

    return result;
}

HOOK(0x004D8B00, TAutonomo_StartTaskAndCopyTargetHandle, "auto");
