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

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

CSF_GLOBAL(uint32_t, g_Value_8B0574, 0x008B0574);

static C_00830438* GameBddLoader_GetDefaultLoadResult() {
    if (g_Value_8B0574 != 0) {
        return reinterpret_cast<C_00830438*>(g_Value_8B0574);
    }

    void* allocated = Mem_Alloc(12);
    if (allocated == nullptr) {
        g_Value_8B0574 = 0;
        return nullptr;
    }

    C_00830438* loadResult = reinterpret_cast<C_00830438*>(allocated);
    void* constructed = C_00830438_Construct(loadResult, nullptr);
    loadResult->vtbl = CSF_VTBL(0x840BA8);
    g_Value_8B0574 = reinterpret_cast<uint32_t>(loadResult);
    return reinterpret_cast<C_00830438*>(constructed);
}

HOOK(0x518940, GameBddLoader_GetDefaultLoadResult, "auto");
