// Commandos: Strike Force - scene loader.
// If the object referenced at +0x12C is non-null, the original tail-jumps to 0x4DEE90
// with that object as `this`. Otherwise it returns without calling anything.

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x4DEE90 : __thiscall, no stack args (ret 0).
CSF_FUNC(uint8_t, __fastcall, C_0083e124_Initialize, 0x004DEE90, (C_0083e124* self, void*));

static int __fastcall SceneLoader_RefreshSceneWeather(void* self, void* /*edx*/)
{
    // The original loads the sub-object pointer into ecx and tail-jumps, so the callee
    // receives the sub-object (not the loader) as `this`, and its eax is returned as-is.
    uint32_t pWeather = CSF_AT(uint32_t, self, 0x12C);
    if (pWeather != 0)
    {
        return C_0083e124_Initialize(reinterpret_cast<C_0083e124*>(pWeather), nullptr);
    }

    // Original leaves eax untouched on this path; the caller-visible value is not
    // reproducible in C++, so 0 is returned.
    return 0;
}

HOOK(0x44F380, SceneLoader_RefreshSceneWeather, "auto");
