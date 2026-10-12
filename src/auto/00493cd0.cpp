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

CSF_FUNC(int, __fastcall, SceneObject_IsSecondCheckTrue, 0x0048E140, (void* self, void* edx));

// Returns whether the scene object passes its base check and has none of the
// flag bits in 0xFFFEF7F8 set at +0x2E0.
static bool __fastcall C_00833908_IsGeometryInteractionEnabled(C_00833908* self, void* /*edx*/)
{
    bool result = SceneObject_IsSecondCheckTrue(self, nullptr) != 0;
    if (result && (CSF_AT(uint32_t, self, 0x2E0) & 0xFFFEF7F8u) != 0)
        return false;
    return result;
}

HOOK(0x493CD0, C_00833908_IsGeometryInteractionEnabled, "auto");
