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

CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));

// Resolves the scene handle for the character's current action: the value in
// the "active" slot (field 0x94), falling back to the "pending" slot (field 0x90).
// Returns 0 when neither slot holds a valid entry.
static int __fastcall CharActionFsm_ResolveSceneHandle(void* self, void* /*edx*/)
{
    int result = 0;

    if (!Hud_IsSuspended(CSF_AT(C_00833908*, self, 0x08), nullptr) ||
        (result = CSF_AT(int, self, 0x84)) == 0)
    {
        int activeSlot = CSF_AT(int, self, 0x94);
        result = 0;
        if (activeSlot != -1)
            result = CSF_AT(int*, self, 0x78)[activeSlot];

        if (!result)
        {
            int pendingSlot = CSF_AT(int, self, 0x90);
            if (pendingSlot != -1)
                return CSF_AT(int*, self, 0x78)[pendingSlot];
        }
    }
    return result;
}

HOOK(0x486F70, CharActionFsm_ResolveSceneHandle, "auto");
