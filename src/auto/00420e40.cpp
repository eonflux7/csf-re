#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x48E510: returns a float-valued weight for a task state (x87 st0 result). Caller stores it as float.
CSF_FUNC(double, __fastcall, AiTaskStates_ComputeStateValue, 0x0048E510, (void* self, void* edx, int a2));

// 0x420E40: derives the task state from the subobject's flag at +0x1E0, notifies the subobject
// through vtable slot 35 (+0x8C), then stores the state's float weight at owner+0x200.
static int __fastcall AiTaskStates_RefreshTaskState(void* self, void* /*edx*/) {
    void* owner = CSF_AT(void*, self, 0x08);
    int subFlag = CSF_AT(int, CSF_AT(void*, owner, 0x14), 0x1E0);

    if (subFlag == 0) {
        CSF_AT(int, owner, 0x238) = 1;
    } else if (subFlag == 1) {
        CSF_AT(int, owner, 0x238) = 3;
    } else if (subFlag == 2) {
        CSF_AT(int, owner, 0x238) = 23;
    }

    void* sub = CSF_AT(void*, owner, 0x14);
    auto notify = reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, sub, 0)[0x8C / 4]);
    notify(sub, nullptr, CSF_AT(int, owner, 0x238), 0, -1, 0);

    int state = CSF_AT(int, owner, 0x238);
    float weight = (float)AiTaskStates_ComputeStateValue(CSF_AT(void*, owner, 0x14), nullptr, state);
    CSF_AT(float, owner, 0x200) = weight;

    return 0;
}

HOOK(0x420E40, AiTaskStates_RefreshTaskState, "auto");
