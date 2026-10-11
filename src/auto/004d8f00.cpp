#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Copies the owner's target handle, drops any pending task target, and when nothing
// is pending, asks the owner to reset its state and returns the task lookup result.
static int __fastcall C_00834fa8_ResetTaskTarget(C_00834fa8* self, void* /*edx*/)
{
    // Stack handle filled by AiAttackTasks_CopyTargetHandle: {vtable, -1, -1}
    alignas(8) int targetHandle[3];
    targetHandle[0] = reinterpret_cast<int>(CSF_VTBL(0x0082D448));
    targetHandle[1] = -1;
    targetHandle[2] = -1;

    uint32_t ownerRefAddr = CSF_AT(uint32_t, self, 0x08);
    void* ownerObj = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(ownerRefAddr), 0x14));
    S_0048c9f0* ownerTarget = reinterpret_cast<S_0048c9f0*>(CSF_AT(uint32_t, ownerObj, 0x204));

    (void)CSF_VTBL(0x0048C9F0);
    AiAttackTasks_CopyTargetHandle(ownerTarget, nullptr, reinterpret_cast<int>(targetHandle));

    S_0047ae10* pendingTask = reinterpret_cast<S_0047ae10*>(CSF_AT(uint32_t, self, 0x18));
    if (pendingTask) {
        (void)CSF_VTBL(0x0047AE10);
        char* nameBuffer = AiCore_GetTaskTargetNameBuffer(pendingTask, nullptr);
        ResetState(nameBuffer, nullptr);
        CSF_AT(uint32_t, self, 0x18) = 0;
    }

    int result = CSF_AT(int, self, 0x14);
    if (result == 0) {
        result = CSF_AT(int, self, 0x18);
        if (result == 0) {
            uint32_t ownerRefAddrAgain = CSF_AT(uint32_t, self, 0x08);
            void* ownerObjAgain = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(ownerRefAddrAgain), 0x14));

            using OwnerResetFn = void(__fastcall*)(void*, void*, int, int, int, int);
            OwnerResetFn ownerReset = reinterpret_cast<OwnerResetFn>(
                CSF_AT(void**, ownerObjAgain, 0)[0x8C / 4]);
            ownerReset(ownerObjAgain, nullptr, 0, 0, -1, 0);

            uint32_t taskLookupArg = CSF_AT(uint32_t, self, 0x0C) + 0x54;
            return CharTask_LookupByOffset(self, nullptr, static_cast<int>(taskLookupArg));
        }
    }
    return result;
}

HOOK(0x004D8F00, C_00834fa8_ResetTaskTarget, "auto");
