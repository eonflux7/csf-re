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

// Clears the pending target and falls back to the static AI's default handling.
// Bound by address; the original's "sub_4AEF70" name is replaced by a descriptive one.
CSF_FUNC(void*, __fastcall, StaticAi_ResolveNoTarget, 0x004AEF70, (void* self, void* edx));

// Resolves the pending target id stored in the static AI's task slot.
// Asks the owning AI object to translate the stored target into a task id; if the
// lookup fails, runs the fallback path, otherwise asks the AI object to start that task.
// Afterwards the pending target is cleared to -1.
static C_008319c0* __fastcall S_004aef70_ResolvePendingTarget(S_004aef70* self, void* /*edx*/)
{
    C_008319c0* result = reinterpret_cast<C_008319c0*>(self->field_068c);

    if (self->field_068c != 0xFFFFFFFFu)
    {
        // Owner pointer lives at field_002c; the AI object it refers to sits at +0x44.
        uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_002c);
        void* aiObj = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x44));

        // Translate the pending target: vtable slot 0xC4 / 4 = 49.
        int lookupIndex =
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, aiObj, 0)[0xC4 / 4])(
                aiObj, nullptr, static_cast<int>(self->field_068c));

        if (lookupIndex == -1)
        {
            result = reinterpret_cast<C_008319c0*>(StaticAi_ResolveNoTarget(self, nullptr));
        }
        else
        {
            // Start the resolved task: vtable slot 0xF8 / 4 = 62 (vtable re-read after the call).
            result = reinterpret_cast<C_008319c0*>(
                ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, aiObj, 0)[0xF8 / 4])(
                    aiObj, nullptr, lookupIndex));
        }

        self->field_068c = 0xFFFFFFFFu;
    }

    return result;
}

HOOK(0x004AF100, S_004aef70_ResolvePendingTarget, "auto");
