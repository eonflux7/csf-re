#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, C_00861988_SetSecondaryParameters, 0x006FD040, (void* self, void* edx, uint32_t* a2));

static void __fastcall C_00862390_UpdateSecondaryParameterTransition(
    C_00862390* self, void* /*edx*/, float elapsedTime)
{
    if (self->field_002c > g_fZero)
    {
        if (elapsedTime > self->field_002c)
        {
            const float adjustment = self->field_0028 * self->field_002c;
            self->field_002c = 0.0f;
            self->field_0024 = adjustment + self->field_0024;
        }
        else
        {
            self->field_0024 =
                elapsedTime * self->field_0028 + self->field_0024;
            self->field_002c = self->field_002c - elapsedTime;
        }

        if (self->field_002c == g_fZero && self->field_0028 < g_fZero)
            self->vSlot2C(1);

        alignas(8) uint8_t parameterBytes[0x10];
        auto* parameters = reinterpret_cast<uint32_t*>(parameterBytes);
        std::memcpy(&parameters[0], &self->field_0024, sizeof(float));
        parameters[1] = parameters[0];
        parameters[2] = parameters[0];
        parameters[3] = 1065353216;

        C_00861988_SetSecondaryParameters(
            reinterpret_cast<void*>(self->field_0020), nullptr, parameters);
    }
}

HOOK(0x006F10C0, C_00862390_UpdateSecondaryParameterTransition, "auto");
