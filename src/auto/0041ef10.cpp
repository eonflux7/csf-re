#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_44D9B0, 0x0044D9B0, (void* self, void* edx, unsigned int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Looks up the pending state id (this[5]) in the current scene's list.
// If present, appends it to the scene's state buffer and returns the new count;
// otherwise returns -1. Always clears the pending id afterwards.
static int __fastcall AiTask_QueueStateIfFound(uint32_t* self, void* /*edx*/)
{
    C_008311f0* scene = CSF_AT(C_008311f0*, g_pCurrentScene, 0x130);
    uint32_t stateId = self[5];
    int result = sub_44D9B0(scene, nullptr, stateId);

    if (result != -1)
    {
        uint32_t usedCount = scene->field_001c;
        uint32_t capacity = CSF_AT(uint32_t, scene, 0x18);
        S_0041e5f0* stateList = reinterpret_cast<S_0041e5f0*>(&scene->field_0014);

        if (usedCount >= capacity)
            GrowButtonArray(stateList, nullptr, stateList->field_000c);

        uint32_t index = stateList->field_0008;
        uint32_t* slot = reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(stateList->field_0000) + 4u * index);
        result = static_cast<int>(index + 1);
        stateList->field_0008 = static_cast<uint32_t>(result);
        *slot = stateId;
    }

    self[5] = 0;
    return result;
}

// The original references S_0041e5f0's vtable via the GrowButtonArray call site.
static void* const s_GrowButtonArrayVtbl = CSF_VTBL(0x0041E5F0);

HOOK(0x41EF10, AiTask_QueueStateIfFound, "auto");
