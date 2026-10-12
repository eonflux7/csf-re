#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GfxScreenEffects_ResetEntryRange, 0x006F8810, (int begin, int end));
CSF_FUNC(uint32_t*, __fastcall, GfxScreenEffects_AddParameter, 0x006F8D50, (void* self, void* edx, int* a3));

static std::uint32_t __fastcall C_00862c74_InitializeIntensityParameter(
    C_00862c74* self, void* /*edx*/)
{
    auto* entries = reinterpret_cast<std::uint8_t*>(self) + 0x50;

    if (self->field_0054 != 0)
    {
        GfxScreenEffects_ResetEntryRange(
            static_cast<int>(self->field_0054),
            CSF_AT(int, self, 0x58));
        if (self->field_0054 != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0054));
    }

    self->field_0054 = 0;
    CSF_AT(std::uint32_t, self, 0x58) = 0;
    CSF_AT(std::uint32_t, self, 0x5C) = 0;

    // One buffer preserves the contiguous parameter and string stack objects.
    // Stack slots use their original frame-relative offsets, not class offsets.
    alignas(8) std::uint8_t frameBytes[0x4C];
    auto* frameEnd = frameBytes + sizeof(frameBytes);
    auto& parameterName =
        *reinterpret_cast<C_0082d410*>(frameEnd - 0x48);
    auto& intensityName =
        *reinterpret_cast<C_0082d410*>(frameEnd - 0x20);

    CSF_AT(std::uint32_t, frameEnd, -0x08) = 15;
    CSF_AT(int, frameEnd, -0x0C) = 0;
    reinterpret_cast<std::uint8_t*>(&intensityName.field_0004)[0] = 0;
    AssignStringRange_429B60(
        &intensityName, nullptr,
        const_cast<char*>(CSF_STR(0x00862ADC, "Intensity")), 9);

    CSF_AT(int, frameEnd, -0x4C) = 0;
    CSF_AT(std::uint32_t, frameEnd, -0x30) = 15;
    CSF_AT(int, frameEnd, -0x34) = 0;
    reinterpret_cast<std::uint8_t*>(&parameterName.field_0004)[0] = 0;
    AssignStringSubstring(
        &parameterName, nullptr, &intensityName, 0,
        static_cast<std::uint32_t>(-1));

    CSF_AT(int, frameEnd, -0x2C) = 0;
    CSF_AT(int, frameEnd, -0x28) = 100;
    CSF_AT(int, frameEnd, -0x24) = 100;
    GfxScreenEffects_AddParameter(
        entries, nullptr, &CSF_AT(int, frameEnd, -0x4C));

    std::uint32_t result = CSF_AT(std::uint32_t, frameEnd, -0x30);
    if (result >= 0x10)
    {
        result = parameterName.field_0004;
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }

    CSF_AT(std::uint32_t, frameEnd, -0x30) = 15;
    CSF_AT(int, frameEnd, -0x34) = 0;
    reinterpret_cast<std::uint8_t*>(&parameterName.field_0004)[0] = 0;

    if (CSF_AT(std::uint32_t, frameEnd, -0x08) >= 0x10)
    {
        result = intensityName.field_0004;
        if (result != 0)
            return Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    return result;
}

HOOK(0x006FB320, C_00862c74_InitializeIntensityParameter, "auto");
