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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, C_0083e160_ApplyPendingEvent, 0x004E21C0, (C_0083e160* self, void* edx, uint32_t* a2));

// Vtable slot 0xB4 (45): no-argument predicate, returns bool in al.
static bool __fastcall C_0083e8c8_IsSuppressed(C_0083e8c8* self, void* /*edx*/)
{
    using Fn = bool(__fastcall*)(void*, void*);
    return ((Fn)CSF_AT(void**, self, 0)[180 / 4])(self, nullptr);
}

// Vtable slot 0x90 (36): three stack args (a2[3], a2[9], a2), returns bool in al.
static bool __fastcall C_0083e8c8_CanAcceptEvent(C_0083e8c8* self, void* /*edx*/,
                                                 uint32_t firstArg, uint32_t secondArg, uint32_t* a2)
{
    using Fn = bool(__fastcall*)(void*, void*, uint32_t, uint32_t, uint32_t*);
    return ((Fn)CSF_AT(void**, self, 0)[144 / 4])(self, nullptr, firstArg, secondArg, a2);
}

// Forwards an event to the base handler when the field is negative,
// the object is not suppressed and the acceptance check passes.
static void __fastcall C_0083e8c8_ForwardNegativeLimitEvent(C_0083e8c8* self, void* /*edx*/, uint32_t* a2)
{
    if (self->field_0044 < g_fZero)
    {
        if (!C_0083e8c8_IsSuppressed(self, nullptr))
        {
            if (C_0083e8c8_CanAcceptEvent(self, nullptr, a2[3], a2[9], a2))
            {
                C_0083e160_ApplyPendingEvent(reinterpret_cast<C_0083e160*>(self), nullptr, a2);
            }
        }
    }
}

HOOK(0x004E2670, C_0083e8c8_ForwardNegativeLimitEvent, "auto");
