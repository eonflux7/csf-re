#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, NextRandomFromState_50DCF0, 0x0050DCF0, (void* self, void* edx));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __stdcall RollValueInRangeFromEndpoints(void* outObj, int /*unused*/, uint32_t* endpoints) {
    // Endpoint values: endpoints[i] -> +4 -> +4 holds a float; truncated to int.
    void* startObj = reinterpret_cast<void*>(endpoints[0]);
    void* startSub = CSF_AT(void*, startObj, 4);
    float startFloat = CSF_AT(float, startSub, 4);
    int rangeStart = (int)startFloat;

    void* endObj = reinterpret_cast<void*>(endpoints[1]);
    void* endSub = CSF_AT(void*, endObj, 4);
    float endFloat = CSF_AT(float, endSub, 4);
    int rangeLength = (int)endFloat - rangeStart + 1;

    if (rangeLength <= 0) {
        void* resultObj = CSF_AT(void*, outObj, 4);
        if (rangeStart < 0) {
            CSF_AT(float, resultObj, 4) = g_fZero;
        } else {
            CSF_AT(float, resultObj, 4) = (float)rangeStart;
        }
        return reinterpret_cast<int>(resultObj);
    }

    // Read the target object before the RNG calls, as the original does.
    void* targetObj = CSF_AT(void*, outObj, 4);
    int randomValue = NextRandomFromState_50DCF0(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
    int offsetInRange = randomValue % rangeLength;
    CSF_AT(float, targetObj, 4) = (float)(rangeStart + offsetInRange);
    return randomValue / rangeLength;
}

HOOK(0x0045C350, RollValueInRangeFromEndpoints, "auto");
