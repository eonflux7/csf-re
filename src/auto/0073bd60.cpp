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

CSF_FUNC(bool, __fastcall, NetSession_IsDuplicate, 0x0064B5C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_74B690, 0x0074B690, (void* self, void* edx, char a2));

// Resets the pending-target slot to -1 when it matches the given id.
// Original: __thiscall, one stack argument, ret 4.
static char __fastcall C_00858180_ResetTargetIfMatches(C_00858180* self, void* /*edx*/, int targetId)
{
    CSF_AT(float, self, 0x9C) = 1.0f;

    char result = static_cast<char>(NetSession_IsDuplicate(self, nullptr));
    if (result)
    {
        return sub_74B690(self, nullptr, static_cast<char>(targetId));
    }

    int currentId = CSF_AT(int, self, 0x5C);
    if (currentId == targetId)
    {
        result = -1;
        if (currentId != -1)
            CSF_AT(int, self, 0x5C) = -1;
    }
    return result;
}

HOOK(0x0073BD60, C_00858180_ResetTargetIfMatches, "auto");
