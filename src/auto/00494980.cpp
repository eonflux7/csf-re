#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x0048E270: __thiscall with one stack argument (ret 4).
CSF_FUNC(int, __fastcall, CPlayerCharacter_InitStateFlag, 0x0048E270, (void* self, void* edx, int a2));

// Returns the cached byte at +0x228 if nonzero; otherwise forwards to the
// initializer at 0x48E270 with the same argument and returns its result.
static char __fastcall CPlayerCharacter_GetStateFlagOrResolve(CPlayerCharacter* self, void* /*edx*/, int a2)
{
    char result = static_cast<char>(self->field_0228);
    if (!result)
        return static_cast<char>(CPlayerCharacter_InitStateFlag(self, nullptr, a2));
    return result;
}

HOOK(0x00494980, CPlayerCharacter_GetStateFlagOrResolve, "auto");
