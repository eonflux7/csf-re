#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static void __fastcall ReplaceAttachedObjectAndCacheParameters(
    void* self, void* /*edx*/, void* replacement)
{
    void* previous = CSF_AT(void*, self, 0x148);
    if (previous == replacement)
        return;

    if (previous)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, previous, 0)[0])(previous, nullptr, 1);
        CSF_AT(void*, self, 0x148) = nullptr;
    }

    CSF_AT(void*, self, 0x148) = replacement;
    if (!replacement)
        return;

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, replacement, 0)[26])(replacement, nullptr, 1);

    void* attached = CSF_AT(void*, self, 0x148);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, attached, 0)[27])(attached, nullptr, 1);

    attached = CSF_AT(void*, self, 0x148);
    void* parameters = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, attached, 0)[25])(attached, nullptr);

    const std::uint32_t firstWord = CSF_AT(std::uint32_t, parameters, 0x00);
    const std::uint32_t secondWord = CSF_AT(std::uint32_t, parameters, 0x04);
    const std::uint32_t thirdWord = CSF_AT(std::uint32_t, parameters, 0x08);
    const std::uint32_t fourthWord = CSF_AT(std::uint32_t, parameters, 0x0C);

    float parameterValue;
    std::memcpy(&parameterValue, &fourthWord, sizeof(parameterValue));
    const float scaledValue = parameterValue * g_HalfDurationScale;

    CSF_AT(std::uint32_t, self, 0x158) = fourthWord;
    CSF_AT(std::uint32_t, self, 0x150) = fourthWord;
    CSF_AT(float, self, 0x154) = scaledValue;

    auto* cachedParameters = static_cast<std::uint8_t*>(self) + 0x15C;
    CSF_AT(std::uint32_t, cachedParameters, 0x00) = firstWord;
    CSF_AT(std::uint32_t, cachedParameters, 0x04) = secondWord;
    CSF_AT(std::uint32_t, cachedParameters, 0x08) = thirdWord;
    CSF_AT(std::uint32_t, cachedParameters, 0x0C) = fourthWord;
}

HOOK(0x00730120, ReplaceAttachedObjectAndCacheParameters, "auto");
