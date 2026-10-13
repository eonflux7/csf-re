#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(double, __fastcall, SceneMissionSetup_EvaluateAnimationTime, 0x0050DD80, (void* self, void* edx, float a2));

// Queues a shot's timing data on the attack task object: stores the two
// input times, their sum (end time), and the peak value, then returns the
// value at +0x14. Original: __thiscall, ret 0x14 (5 stack dwords).
static unsigned int __fastcall AttackTasks_QueueShot(void* self, void* /*edx*/,
                                                     int a2, float a3, float a4,
                                                     int a5, int a6)
{
    float endTime = a3 + a4;

    CSF_AT(float, self, 0x0C) = a3;
    CSF_AT(int, self, 0x08) = a2;
    CSF_AT(float, self, 0x10) = a4;
    CSF_AT(int, self, 0x14) = a5;
    CSF_AT(int, self, 0x18) = a6;

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    double evaluated = SceneMissionSetup_EvaluateAnimationTime(globalState, nullptr, endTime);
    CSF_AT(float, self, 0x04) = static_cast<float>(evaluated);

    unsigned int result = static_cast<unsigned int>(CSF_AT(int, self, 0x14));
    if (static_cast<unsigned int>(CSF_AT(int, self, 0x18)) < result) {
        CSF_AT(int, self, 0x18) = static_cast<int>(result);
    }
    return result;
}

HOOK(0x0048C630, AttackTasks_QueueShot, "auto");
