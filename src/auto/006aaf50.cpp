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

// Callees and data bound by original address.
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(int, __fastcall, sub_652810, 0x00652810, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_6AAAE0, 0x006AAAE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_7A0C70, 0x007A0C70, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_7A14C0, 0x007A14C0, (void* self, void* edx));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Vtable of the class whose method sub_6AAAE0 belongs to; referenced by address.
static void* const s_TeardownVtable = CSF_VTBL(0x006AAAE0);

static void __fastcall S_006aaae0_TickAndRelease(S_006aaae0* self, void* /*edx*/, float elapsed)
{
    // Countdown: subtract elapsed time and store the remainder.
    float remaining = self->field_002c - elapsed;
    self->field_002c = remaining;

    if (remaining < g_fZero)
    {
        if (self->field_0010)
        {
            int result = sub_7A0C70(reinterpret_cast<void*>(self->field_0028), nullptr,
                                    reinterpret_cast<int>(self->pad_0008));
            *reinterpret_cast<int*>(self->field_0030) = result;
        }
        else
        {
            *reinterpret_cast<int*>(self->field_0030) = -1;
        }

        if (*reinterpret_cast<int*>(self->field_0030) < 0)
        {
            sub_6AAAE0(self, nullptr);
        }

        uint32_t storage = self->field_0028;
        if (storage)
        {
            sub_7A14C0(reinterpret_cast<void*>(storage), nullptr);
            NetSession_ReleaseTransportStorage(static_cast<int>(storage));
            self->field_0028 = 0;
        }

        sub_652810(self->pad_0008, nullptr);
        self->field_0004 = 0;
    }
}

HOOK(0x006AAF50, S_006aaae0_TickAndRelease, "auto");
