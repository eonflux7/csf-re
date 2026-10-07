#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall C_008658f8_UpdateWithScaledFourthComponent(
    C_008658f8* self, void* /*edx*/)
{
    if (self->field_0030 == 0)
        return 0;

    const uint8_t enabled = self->field_0034;
    if (enabled == 0)
        return 0;

    void* object = reinterpret_cast<void*>(self->field_0030);
    void* state =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[12])(object, nullptr);

    uint32_t originalState[4];
    originalState[0] = CSF_AT(uint32_t, state, 0x00);
    originalState[1] = CSF_AT(uint32_t, state, 0x04);
    originalState[2] = CSF_AT(uint32_t, state, 0x08);
    originalState[3] = CSF_AT(uint32_t, state, 0x0C);

    float fourthComponent;
    std::memcpy(&fourthComponent, &originalState[3], sizeof(float));
    const float scaledComponent = fourthComponent * self->field_0038;

    uint32_t scaledState[4];
    scaledState[0] = originalState[0];
    object = reinterpret_cast<void*>(self->field_0030);
    std::memcpy(&scaledState[3], &scaledComponent, sizeof(float));
    scaledState[1] = originalState[1];
    scaledState[2] = originalState[2];

    ((int(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, object, 0)[11])(object, nullptr, scaledState);

    object = reinterpret_cast<void*>(self->field_0030);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, object, 0)[2])(object, nullptr);

    object = reinterpret_cast<void*>(self->field_0030);
    return static_cast<char>(
        ((int(__fastcall*)(void*, void*, uint32_t*))
            CSF_AT(void**, object, 0)[11])(object, nullptr, originalState));
}

HOOK(0x00728780, C_008658f8_UpdateWithScaledFourthComponent, "auto");
