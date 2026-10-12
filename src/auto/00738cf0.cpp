#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_737310, 0x00737310, (void* self, void* edx, float* a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static void __fastcall C_008673b0_SetClampedComponents(
    C_008673b0* self, void* /*edx*/, float* components)
{
    if (static_cast<std::uint8_t>(
            sub_737310(&self->field_001c, nullptr, components)) == 0)
        return;

    self->field_001c = components[0];
    self->field_0020 = components[1];
    self->field_0024 = components[2];
    self->field_0028 = components[3];

    if (self->field_001c < g_fZero)
        self->field_001c = 0.0f;
    if (self->field_001c > g_TransitionOne)
        self->field_001c = 1.0f;

    if (self->field_0024 < g_fZero)
        self->field_0024 = 0.0f;
    if (self->field_0024 > g_TransitionOne)
        self->field_0024 = 1.0f;

    if (self->field_0020 < g_fZero)
        self->field_0020 = 0.0f;
    if (self->field_0020 > g_TransitionOne)
        self->field_0020 = 1.0f;

    if (self->field_0028 < g_fZero)
        self->field_0028 = 0.0f;
    if (self->field_0028 > g_TransitionOne)
        self->field_0028 = 1.0f;
}

HOOK(0x00738CF0, C_008673b0_SetClampedComponents, "auto");
