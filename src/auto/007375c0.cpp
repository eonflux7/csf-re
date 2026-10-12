#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AreFloat4ValuesDifferent, 0x00737310,
         (void* self, void* edx, float* a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static void __fastcall C_00867260_SetClampedComponents(
    C_00867260* self, void* /*edx*/, float* components)
{
    float* stored = &CSF_AT(float, self, 0x58);
    if (static_cast<std::uint8_t>(
            AreFloat4ValuesDifferent(stored, nullptr, components)) == 0)
        return;

    CSF_AT(std::uint8_t, self, 0x20) = 1;
    stored[0] = components[0];
    stored[1] = components[1];
    stored[2] = components[2];
    stored[3] = components[3];

    if (CSF_AT(float, self, 0x58) < g_fZero)
        CSF_AT(float, self, 0x58) = 0.0f;
    if (CSF_AT(float, self, 0x58) > g_TransitionOne)
        CSF_AT(float, self, 0x58) = 1.0f;

    if (CSF_AT(float, self, 0x60) < g_fZero)
        CSF_AT(float, self, 0x60) = 0.0f;
    if (CSF_AT(float, self, 0x60) > g_TransitionOne)
        CSF_AT(float, self, 0x60) = 1.0f;

    if (CSF_AT(float, self, 0x5C) < g_fZero)
        CSF_AT(float, self, 0x5C) = 0.0f;
    if (CSF_AT(float, self, 0x5C) > g_TransitionOne)
        CSF_AT(float, self, 0x5C) = 1.0f;

    if (CSF_AT(float, self, 0x64) < g_fZero)
        CSF_AT(float, self, 0x64) = 0.0f;
    if (CSF_AT(float, self, 0x64) > g_TransitionOne)
        CSF_AT(float, self, 0x64) = 1.0f;
}

HOOK(0x007375C0, C_00867260_SetClampedComponents, "auto");
