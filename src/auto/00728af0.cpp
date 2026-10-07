#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00865a10_InvokeWithScaledFourthComponent(
    C_00865a10* self, void* /*edx*/, void* object)
{
    void* state = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[27])(object, nullptr);

    std::uint32_t originalState[4];
    originalState[0] = CSF_AT(std::uint32_t, state, 0x00);
    originalState[1] = CSF_AT(std::uint32_t, state, 0x04);
    originalState[2] = CSF_AT(std::uint32_t, state, 0x08);
    originalState[3] = CSF_AT(std::uint32_t, state, 0x0C);

    float fourthComponent;
    std::memcpy(&fourthComponent, &originalState[3], sizeof(float));
    const float scaledComponent = fourthComponent * self->field_0044;

    std::uint32_t scaledState[4];
    scaledState[0] = originalState[0];
    scaledState[1] = originalState[1];
    scaledState[2] = originalState[2];
    std::memcpy(&scaledState[3], &scaledComponent, sizeof(float));

    reinterpret_cast<int (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, object, 0)[26])(object, nullptr, scaledState);

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[2])(object, nullptr);

    return reinterpret_cast<int (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, object, 0)[26])(object, nullptr, originalState);
}

HOOK(0x00728AF0, C_00865a10_InvokeWithScaledFourthComponent, "auto");
