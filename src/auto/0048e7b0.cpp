#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

static int* __fastcall GetDisplayColor(void* self, void* /*edx*/, int* output)
{
    float defaultColor[4] = {0.4f, 0.4f, 0.4f, 1.0f};
    const void* color = defaultColor;

    if (CSF_AT(int, self, 0x50) == 32 &&
        (CSF_AT(std::uint32_t, CSF_AT(void*, self, 0x34), 0x40) & 4) != 0)
    {
        void* provider =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);

        color = reinterpret_cast<void* (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, provider, 0)[0x60 / 4])(
                provider, nullptr, defaultColor);
    }

    int components[4];
    std::memcpy(components, color, sizeof(components));
    output[0] = components[0];
    output[1] = components[1];
    output[2] = components[2];
    output[3] = components[3];
    return output;
}

HOOK(0x0048E7B0, GetDisplayColor, "auto");
