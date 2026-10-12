#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GfxScreenEffects_ResetEntryRange, 0x006F8810, (int begin, int end));
CSF_FUNC(uint32_t*, __fastcall, GfxScreenEffects_AddParameter, 0x006F8D50, (void* self, void* edx, int* a3));

static uint32_t __fastcall C_00862b04_ResetIntensityParameters(
    C_00862b04* self, void* /*edx*/)
{
    auto* entries = reinterpret_cast<std::uint8_t*>(self) + 0x50;
    const int begin = CSF_AT(int, self, 0x54);
    if (begin)
    {
        GfxScreenEffects_ResetEntryRange(begin, CSF_AT(int, entries, 0x08));
        if (CSF_AT(uint32_t, entries, 0x04))
            Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, entries, 0x04)));
    }

    CSF_AT(uint32_t, entries, 0x04) = 0;
    CSF_AT(uint32_t, entries, 0x08) = 0;
    CSF_AT(uint32_t, entries, 0x0C) = 0;

    // One allocation preserves the contiguous parameter record and both strings.
    alignas(8) std::uint8_t storage[0x4C];
    auto* storageEnd = storage + sizeof(storage);
    int& parameterId = CSF_AT(int, storageEnd, -0x4C);
    auto& parameterName =
        *reinterpret_cast<C_0082d410*>(storageEnd - 0x48);
    auto& intensityName =
        *reinterpret_cast<C_0082d410*>(storageEnd - 0x20);

    CSF_AT(uint32_t, storageEnd, -0x08) = 15;
    CSF_AT(int, storageEnd, -0x0C) = 0;
    reinterpret_cast<std::uint8_t*>(&intensityName.field_0004)[0] = 0;
    AssignStringRange_429B60(
        &intensityName, nullptr,
        const_cast<char*>(CSF_STR(0x00862ADC, "Intensity")), 9);

    parameterId = 0;
    CSF_AT(uint32_t, storageEnd, -0x30) = 15;
    CSF_AT(int, storageEnd, -0x34) = 0;
    reinterpret_cast<std::uint8_t*>(&parameterName.field_0004)[0] = 0;
    AssignStringSubstring(&parameterName, nullptr, &intensityName, 0, -1);

    CSF_AT(int, storageEnd, -0x2C) = 0;
    CSF_AT(int, storageEnd, -0x28) = 100;
    CSF_AT(int, storageEnd, -0x24) = 10;
    GfxScreenEffects_AddParameter(entries, nullptr, &parameterId);

    uint32_t result = CSF_AT(uint32_t, storageEnd, -0x30);
    if (result >= 0x10)
    {
        result = parameterName.field_0004;
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }

    CSF_AT(uint32_t, storageEnd, -0x30) = 15;
    CSF_AT(int, storageEnd, -0x34) = 0;
    reinterpret_cast<std::uint8_t*>(&parameterName.field_0004)[0] = 0;

    if (CSF_AT(uint32_t, storageEnd, -0x08) >= 0x10)
    {
        result = intensityName.field_0004;
        if (result)
            return Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    return result;
}

HOOK(0x006F9B00, C_00862b04_ResetIntensityParameters, "auto");
