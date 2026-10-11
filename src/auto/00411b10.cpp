#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static void __fastcall C_0082dfd0_ReduceVectorMagnitude(
    C_0082dfd0* self, void* /*edx*/, int /*unused*/)
{
    if (self->field_0090 != g_ZeroVectorX ||
        CSF_AT(float, self, 0x94) != g_ZeroVectorY ||
        CSF_AT(float, self, 0x98) != g_ZeroVectorZ)
    {
        const float magnitude = std::sqrt(
            self->field_0090 * self->field_0090 +
            CSF_AT(float, self, 0x94) * CSF_AT(float, self, 0x94) +
            CSF_AT(float, self, 0x98) * CSF_AT(float, self, 0x98));

        void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        const float remaining = magnitude - CSF_AT(float, settings, 0x934);

        if (!(remaining > g_fZero))
        {
            self->field_0090 = g_ZeroVectorX;
            CSF_AT(float, self, 0x94) = g_ZeroVectorY;
            CSF_AT(float, self, 0x98) = g_ZeroVectorZ;
            CSF_AT(float, self, 0x9C) = g_ZeroVectorX;
            CSF_AT(float, self, 0xA0) = g_ZeroVectorY;
            CSF_AT(float, self, 0xA4) = g_ZeroVectorZ;
        }
        else
        {
            const float scale = remaining / magnitude;
            self->field_0090 = scale * self->field_0090;
            CSF_AT(float, self, 0x94) = scale * CSF_AT(float, self, 0x94);
            CSF_AT(float, self, 0x98) = scale * CSF_AT(float, self, 0x98);
        }
    }
}

HOOK(0x00411B10, C_0082dfd0_ReduceVectorMagnitude, "auto");
