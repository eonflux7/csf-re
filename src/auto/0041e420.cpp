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

CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angle));

// __thiscall-style: ecx = this, one float stack argument, ret 4.
static int __fastcall SetAngleFieldsFromNormalizedAngle(void* self, void* /*edx*/, float angle)
{
    double normalizedAngle = MenuMpCharacterSelect_NormalizeAngle(angle);
    CSF_AT(float, self, 0x08) = (float)normalizedAngle;
    CSF_AT(float, self, 0x04) = (float)normalizedAngle;
    // The original returns eax as left by the callee (value is not meaningful here).
    return 0;
}

HOOK(0x41E420, SetAngleFieldsFromNormalizedAngle, "auto");
