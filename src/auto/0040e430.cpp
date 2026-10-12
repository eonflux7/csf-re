#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_OneOver3600, 0x0082DD70);
CSF_GLOBAL(float, g_Scale100000, 0x0082DD6C);
CSF_GLOBAL(float, g_ContextualDistanceScale, 0x0082DD74);

// Stores a scaled copy of the global value at offset 0x1D70 into the float at +0x50.
// Positive input: +value; negative input: -(value * g_ContextualDistanceScale); zero/NaN: no write.
static void __fastcall ApplyFloatToSubObject(float* self, void* /*edx*/, float a2)
{
    if (!(a2 > g_fZero))
    {
        if (a2 < g_fZero)
        {
            void* globals = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            float scaled = CSF_AT(float, globals, 0x1D70) * g_OneOver3600 * g_Scale100000 * g_ContextualDistanceScale;
            CSF_AT(float, self, 0x50) = -scaled;
        }
    }
    else
    {
        void* globals = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        CSF_AT(float, self, 0x50) = CSF_AT(float, globals, 0x1D70) * g_OneOver3600 * g_Scale100000;
    }
}

HOOK(0x0040E430, ApplyFloatToSubObject, "auto");
