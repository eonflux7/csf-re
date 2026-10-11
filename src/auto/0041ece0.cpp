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

// Callees (bound by address).
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_44D9B0, 0x0044D9B0, (void* self, void* edx, unsigned int a2));

// Data.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Vtable of the list sub-object (S_0041e5f0), referenced by the call site.
[[maybe_unused]] static const void* const kButtonArrayVtbl = CSF_VTBL(0x0041E5F0);

// Appends a (non-zero) id to the current scene's id list, growing it when full,
// but only if sub_44D9B0 accepts the id (returns != -1).
static void __stdcall SceneList_AppendIdIfFound(unsigned int a2)
{
    if (a2)
    {
        C_008311f0* sceneList = CSF_AT(C_008311f0*, g_pCurrentScene, 0x130);
        if (sub_44D9B0(sceneList, nullptr, a2) != -1)
        {
            uint32_t count = sceneList->field_001c;
            uint32_t capacity = CSF_AT(uint32_t, sceneList, 0x18);
            S_0041e5f0* array = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(sceneList) + 0x14);
            if (count >= capacity)
                GrowButtonArray(array, nullptr, array->field_000c);
            uint32_t index = array->field_0008;
            uint32_t* slot = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(array->field_0000) + 4 * index);
            array->field_0008 = index + 1;
            *slot = a2;
        }
    }
}

HOOK(0x0041ECE0, SceneList_AppendIdIfFound, "auto");
