#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00860a30_SetFlagAndOrBits, 0x006C9FB0, (C_00860a30* self, void*));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, GfxPrtStd_GetStateResource, 0x006E04B0, (void* self, void* edx, void* a1, int a2));
CSF_FUNC(float*, __fastcall, C_00862748_UpdateSpecialParticles, 0x006F46C0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00862748_UpdateStandardParticles, 0x006F4870, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_00862748_UpdateParticles(C_00862748* self, void* /*edx*/)
{
    C_00860a30_SetFlagAndOrBits(self, nullptr);

    if (self->field_002c == 1)
    {
        const int resourceId =
            CSF_AT(int, reinterpret_cast<void*>(self->field_0038), 0x10);
        void* manager = reinterpret_cast<void*>(GetValue_8BB738());
        self->field_0024 = GfxPrtStd_GetStateResource(
            manager, nullptr, reinterpret_cast<void*>(1), resourceId);
        C_00862748_UpdateSpecialParticles(self, nullptr);
        return;
    }

    auto* state = reinterpret_cast<S_006dcf90*>(self->field_0030);
    switch (state->field_0100)
    {
    case 0:
        self->field_002c = 0;
        break;
    case 1:
    case 2:
        self->field_002c = 2;
        break;
    default:
        break;
    }

    if (state->field_0004)
        self->field_002c = 3;

    const int resourceId = static_cast<int>(state->field_0008);
    void* resourceState = reinterpret_cast<void*>(self->field_002c);
    void* manager = reinterpret_cast<void*>(GetValue_8BB738());
    self->field_0024 = GfxPrtStd_GetStateResource(
        manager, nullptr, resourceState, resourceId);

    C_00862748_UpdateStandardParticles(self, nullptr);

    void* controller = reinterpret_cast<void*>(self->field_0018);
    if (controller &&
        !CSF_AT(uint8_t, controller, 0x41) &&
        (CSF_AT(uint8_t, controller, 0x44) ||
         reinterpret_cast<const float&>(
             reinterpret_cast<S_006dcf90*>(self->field_0030)->field_0014) == g_fZero))
    {
        self->field_0014 |= 0x20u;
    }
}

HOOK(0x006F4D40, C_00862748_UpdateParticles, "auto");
