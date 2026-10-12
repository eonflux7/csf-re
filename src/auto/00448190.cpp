#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(double, __fastcall, MenuSystem_GetIndexedGlobalFloat, 0x0050DA90, (void* self, void*));

static void __fastcall AiDistracted_ResetReactionTimer(float* self, void* /*edx*/)
{
    int globalState = GetGlobalState_50D190();
    CSF_AT(float, self, 0x60) = static_cast<float>(
        MenuSystem_GetIndexedGlobalFloat(reinterpret_cast<void*>(globalState), nullptr));
}

HOOK(0x00448190, AiDistracted_ResetReactionTimer, "auto");
