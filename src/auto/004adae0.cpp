// CoverAttack_CheckState: decides whether a handle-referenced object can be
// attacked/covered, and if so forwards the check to a child object's virtual method.
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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* edx, std::int32_t candidate));

static bool __fastcall CoverAttack_CheckState_4ADAE0(S_004ad990* self, void* /*edx*/, int a2)
{
    // Handle must be non-negative: [a2 + 8] is read as a signed int.
    if (CSF_AT(int, reinterpret_cast<void*>(a2), 8) < 0)
        return false;

    // Resolve the handle against the current scene (ecx = g_pCurrentScene, push a2).
    int resolvedValue = CHandleTable_Resolve(g_pCurrentScene, nullptr, a2);
    if (resolvedValue == 0)
        return false;
    void* resolved = reinterpret_cast<void*>(resolvedValue);

    // The original loads owner->field_0078 before the flag test; the load is kept in place.
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0000);
    int candidate = CSF_AT(int, owner, 0x78);

    if ((CSF_AT(uint32_t, resolved, 0x170) & 0x2000) != 0)
    {
        if ((IsDistinctNonSentinelValue(resolved, nullptr, candidate) & 0xFF) != 0)
        {
            // Re-read the child pointer after the call, as the original does.
            owner = reinterpret_cast<uint8_t*>(self->field_0000);
            void* child = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x3c));

            // Virtual slot 0x9C / 4 = 39, called __thiscall with (a2, 14).
            using ChildCheckFn = char(__fastcall*)(void* self, void* edx, int a, int b);
            ChildCheckFn fn = reinterpret_cast<ChildCheckFn>(CSF_AT(void**, child, 0)[0x9c / 4]);
            return fn(child, nullptr, a2, 14) != 0;
        }
    }
    return false;
}

HOOK(0x4ADAE0, CoverAttack_CheckState_4ADAE0, "auto");
