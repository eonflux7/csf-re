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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, sub_6B1A30, 0x006B1A30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4DEA50, 0x004DEA50, (void* self, void* edx));

// Forwards the global int object and this to the helper, then if the flag at +4
// is set, runs the helper at 0x4DEA50 and clears the flag. Returns the flag's
// pre-clear value (as the low byte of the helper's result when it was set).
static char __fastcall C_0083e124_ConsumeFlagAndNotify(C_0083e124* self, void* /*edx*/)
{
    C_0083e124* globalObj = reinterpret_cast<C_0083e124*>(GetGlobalInt());
    sub_6B1A30(globalObj, nullptr, reinterpret_cast<int>(self));

    char result = static_cast<char>(self->field_0004);
    if (result)
    {
        result = static_cast<char>(sub_4DEA50(self, nullptr));
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x004DEAA0, C_0083e124_ConsumeFlagAndNotify, "auto");
