#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
static CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_008316d8_UpdateTimers(
    C_008316d8* self, void* /*edx*/, float elapsed)
{
    uint32_t elapsedBits;
    std::memcpy(&elapsedBits, &elapsed, sizeof(elapsedBits));
    self->vSlotD4(reinterpret_cast<void*>(elapsedBits));

    if (self->field_0016)
    {
        self->field_0018 += elapsed;
        void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        if (self->field_0018 > CSF_AT(float, settings, 0xAF4))
        {
            settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            self->field_0018 = CSF_AT(float, settings, 0xAF4);
        }
    }
    else
    {
        const float remaining = self->field_0018 - elapsed;
        self->field_0018 = remaining;
        if (remaining < g_fZero)
            self->field_0018 = 0.0f;
    }

    if (!self->field_0014)
    {
        const float remaining = self->field_0010 - elapsed;
        self->field_0010 = remaining;
        if (remaining < g_fZero)
        {
            self->field_0010 = 0.0f;
            self->field_0014 = 1;
        }
    }
}

HOOK(0x00475B90, C_008316d8_UpdateTimers, "auto");
