#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GfxScreenEffects_ResetEntryRange, 0x006F8810, (int begin, int end));
CSF_FUNC(uint32_t*, __fastcall, GfxScreenEffects_AppendParameter, 0x006F8D50, (void* self, void* edx, int* a3));

static void GfxScreenEffects_InitializeEmptyString(
    C_0082d410& string, int& length, unsigned int& capacity)
{
    capacity = 15;
    length = 0;
    *reinterpret_cast<unsigned char*>(&string.field_0004) = 0;
}

static std::uint32_t GfxScreenEffects_FreeStringStorage(
    C_0082d410& string, const unsigned int& capacity)
{
    std::uint32_t result = capacity;
    if (capacity >= 0x10)
    {
        result = string.field_0004;
        if (string.field_0004)
            result = static_cast<std::uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(string.field_0004)));
    }
    return result;
}

static std::uint32_t __fastcall C_00862d48_InitializeEffectParameters(
    C_00862d48* self, void* /*edx*/)
{
    void* parameters = reinterpret_cast<std::uint8_t*>(self) + 0x50;
    int begin = CSF_AT(int, self, 0x54);
    if (begin)
    {
        GfxScreenEffects_ResetEntryRange(begin, CSF_AT(int, self, 0x58));
        if (CSF_AT(std::uint32_t, self, 0x54))
            Mem_FreeDebug(reinterpret_cast<void*>(
                CSF_AT(std::uint32_t, self, 0x54)));
    }
    CSF_AT(std::uint32_t, self, 0x54) = 0;
    CSF_AT(std::uint32_t, self, 0x58) = 0;
    CSF_AT(std::uint32_t, self, 0x5C) = 0;

    // Keep the ID and parameter data contiguous, with the original stack room.
    alignas(8) std::uint8_t frameBytes[0x4C];
    std::uint8_t* frameEnd = frameBytes + sizeof(frameBytes);
    int& parameterId = *reinterpret_cast<int*>(frameEnd - 0x4C);
    C_0082d410& parameterName =
        *reinterpret_cast<C_0082d410*>(frameEnd - 0x48);
    int& parameterLength = *reinterpret_cast<int*>(frameEnd - 0x34);
    unsigned int& parameterCapacity =
        *reinterpret_cast<unsigned int*>(frameEnd - 0x30);
    int& minimum = *reinterpret_cast<int*>(frameEnd - 0x2C);
    int& maximum = *reinterpret_cast<int*>(frameEnd - 0x28);
    int& defaultValue = *reinterpret_cast<int*>(frameEnd - 0x24);
    C_0082d410& temporaryName =
        *reinterpret_cast<C_0082d410*>(frameEnd - 0x20);
    int& temporaryLength = *reinterpret_cast<int*>(frameEnd - 0x0C);
    unsigned int& temporaryCapacity =
        *reinterpret_cast<unsigned int*>(frameEnd - 0x08);

    GfxScreenEffects_InitializeEmptyString(
        temporaryName, temporaryLength, temporaryCapacity);
    AssignStringRange_429B60(
        &temporaryName, nullptr,
        const_cast<char*>(CSF_STR(0x00862D20, "Cut Color")), 9);

    parameterId = 0;
    GfxScreenEffects_InitializeEmptyString(
        parameterName, parameterLength, parameterCapacity);
    AssignStringSubstring(&parameterName, nullptr, &temporaryName, 0, -1);
    minimum = 1;
    maximum = 255;
    defaultValue = 20;
    GfxScreenEffects_AppendParameter(parameters, nullptr, &parameterId);

    GfxScreenEffects_FreeStringStorage(parameterName, parameterCapacity);
    GfxScreenEffects_InitializeEmptyString(
        parameterName, parameterLength, parameterCapacity);
    GfxScreenEffects_FreeStringStorage(temporaryName, temporaryCapacity);
    GfxScreenEffects_InitializeEmptyString(
        temporaryName, temporaryLength, temporaryCapacity);

    AssignStringRange_429B60(
        &temporaryName, nullptr,
        const_cast<char*>(CSF_STR(0x00862ADC, "Intensity")), 9);

    parameterId = 1;
    GfxScreenEffects_InitializeEmptyString(
        parameterName, parameterLength, parameterCapacity);
    AssignStringSubstring(&parameterName, nullptr, &temporaryName, 0, -1);
    minimum = 1;
    maximum = 1024;
    defaultValue = 60;
    GfxScreenEffects_AppendParameter(parameters, nullptr, &parameterId);

    GfxScreenEffects_FreeStringStorage(parameterName, parameterCapacity);
    GfxScreenEffects_InitializeEmptyString(
        parameterName, parameterLength, parameterCapacity);
    GfxScreenEffects_FreeStringStorage(temporaryName, temporaryCapacity);
    GfxScreenEffects_InitializeEmptyString(
        temporaryName, temporaryLength, temporaryCapacity);

    AssignStringRange_429B60(
        &temporaryName, nullptr,
        const_cast<char*>(CSF_STR(0x00862D10, "Displacement")), 12);

    parameterId = 2;
    GfxScreenEffects_InitializeEmptyString(
        parameterName, parameterLength, parameterCapacity);
    AssignStringSubstring(&parameterName, nullptr, &temporaryName, 0, -1);
    minimum = 0;
    maximum = 100;
    defaultValue = 38;
    GfxScreenEffects_AppendParameter(parameters, nullptr, &parameterId);

    std::uint32_t result =
        GfxScreenEffects_FreeStringStorage(parameterName, parameterCapacity);
    GfxScreenEffects_InitializeEmptyString(
        parameterName, parameterLength, parameterCapacity);
    if (temporaryCapacity >= 0x10)
        result = GfxScreenEffects_FreeStringStorage(
            temporaryName, temporaryCapacity);
    return result;
}

HOOK(0x006FBE30, C_00862d48_InitializeEffectParameters, "auto");
