#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00508360_ReleaseResources, 0x00508360, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00508360_PrepareState, 0x00507730, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, S_00508360_EvaluateWithArg, 0x00508C50, (void* self, void* edx, int a2));

static bool __fastcall S_00508360_ReinitializeAndStoreResult(S_00508360* self, void* /*edx*/, int a2)
{
    if (self->field_0000)
    {
        S_00508360_ReleaseResources(self, nullptr);
        self->field_0000 = 0;
    }
    S_00508360_PrepareState(self, nullptr);
    // ecx was last set to `self` by `mov ecx, esi` before the call to 0x508C50
    bool result = S_00508360_EvaluateWithArg(self, nullptr, a2);
    self->field_0000 = result;
    return result;
}

HOOK(0x00508CB0, S_00508360_ReinitializeAndStoreResult, "auto");
