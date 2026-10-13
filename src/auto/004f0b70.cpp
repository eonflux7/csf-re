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

// Parses one scene-object/lights block via the parser at 0x4F09F0.
// Original: __thiscall, 2 stack args (a2, a3), ret 8.
// ecx keeps `this` unchanged and a2 is passed as the callee's single stack argument.
CSF_FUNC(char, __fastcall, SceneLightsObjects_ParseBlock, 0x004F09F0, (void* self, void* edx, void* a2));

static char __fastcall C_0083ef04_ParseBlockIfNonZero(C_0083ef04* self, void* /*edx*/, S_00692b80* a2, int a3)
{
    char result = 0;
    if (a3)
    {
        // ecx = this (unchanged); the callee is __thiscall with one stack arg (a2), so edx is unused.
        return SceneLightsObjects_ParseBlock(self, nullptr, a2);
    }
    return result;
}

HOOK(0x004F0B70, C_0083ef04_ParseBlockIfNonZero, "auto");
