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

CSF_FUNC(unsigned int, __fastcall, RunStepOneHandler, 0x004D3830, (void* self, void* edx));
CSF_FUNC(void, __fastcall, RunStepTwoHandler, 0x004D3870, (void* self, void* edx));
CSF_FUNC(int, __fastcall, RunStepThreeHandler, 0x004D3BF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, RunStepFourHandler, 0x004D3D30, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetCachedProgressBarControl, 0x0050E220, (void* self, void*));

// Dispatches a loading-step action (1..5) on the object; other values do nothing.
static void __fastcall DispatchLoadingStep(void* self, void* /*edx*/, int actionIndex)
{
    switch (actionIndex)
    {
    case 1:
        RunStepOneHandler(self, nullptr);
        break;
    case 2:
        RunStepTwoHandler(self, nullptr);
        break;
    case 3:
        RunStepThreeHandler(self, nullptr);
        break;
    case 4:
        RunStepFourHandler(self, nullptr);
        break;
    case 5:
    {
        CSF_AT(int, self, 0x40) = 0;
        void* globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
        CSF_AT(int, globalState, 0xFC) = 0;
        void* globalStateForProgress = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
        GetCachedProgressBarControl(globalStateForProgress, nullptr);
        break;
    }
    default:
        return;
    }
}

HOOK(0x004D3E00, DispatchLoadingStep, "auto");
