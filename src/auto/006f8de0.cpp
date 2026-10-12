#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GfxScreenEffects_ResetEntryRange, 0x006F8810, (int begin, int end));
CSF_FUNC(uint32_t*, __fastcall, GfxScreenEffects_AddParameter, 0x006F8D50, (void* self, void* edx, int* a3));

static std::uint32_t __fastcall C_008629f8_InitializeOscillationParameters(
    C_008629f8* self, void* /*edx*/)
{
    alignas(8) std::uint8_t frameBytes[0x50];
    auto* frameEnd = frameBytes + sizeof(frameBytes);
    int& parameterId = CSF_AT(int, frameEnd, -0x50);
    auto& parameterName =
        *reinterpret_cast<C_0082d410*>(frameEnd - 0x4C);
    auto& label =
        *reinterpret_cast<C_0082d410*>(frameEnd - 0x24);

    int& parameterLength = CSF_AT(int, frameEnd, -0x38);
    std::uint32_t& parameterCapacity =
        CSF_AT(std::uint32_t, frameEnd, -0x34);
    int& parameterMinimum = CSF_AT(int, frameEnd, -0x30);
    int& parameterMaximum = CSF_AT(int, frameEnd, -0x2C);
    int& parameterDefault = CSF_AT(int, frameEnd, -0x28);
    int& labelLength = CSF_AT(int, frameEnd, -0x10);
    std::uint32_t& labelCapacity =
        CSF_AT(std::uint32_t, frameEnd, -0x0C);
    int& sizeYMaximum = CSF_AT(int, frameEnd, -0x04);

    auto* parameters = reinterpret_cast<std::uint8_t*>(self) + 0x50;
    if (self->field_0054)
    {
        GfxScreenEffects_ResetEntryRange(
            static_cast<int>(self->field_0054),
            CSF_AT(int, parameters, 0x08));
        if (self->field_0054)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0054));
    }
    self->field_0054 = 0;
    CSF_AT(std::uint32_t, parameters, 0x08) = 0;
    CSF_AT(std::uint32_t, parameters, 0x0C) = 0;

    labelCapacity = 15;
    labelLength = 0;
    reinterpret_cast<std::uint8_t*>(&label.field_0004)[0] = 0;
    AssignStringRange_429B60(
        &label, nullptr, const_cast<char*>(CSF_STR(0x00862980, "Size X")), 6);

    parameterId = 0;
    parameterCapacity = 15;
    parameterLength = 0;
    reinterpret_cast<std::uint8_t*>(&parameterName.field_0004)[0] = 0;
    AssignStringSubstring(&parameterName, nullptr, &label, 0, -1);
    parameterMinimum = 0;
    parameterMaximum = 2096;
    parameterDefault = 200;
    GfxScreenEffects_AddParameter(parameters, nullptr, &parameterId);

    if (parameterCapacity >= 0x10 && parameterName.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(parameterName.field_0004));
    parameterCapacity = 15;
    parameterLength = 0;
    reinterpret_cast<std::uint8_t*>(&parameterName.field_0004)[0] = 0;

    if (labelCapacity >= 0x10 && label.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(label.field_0004));
    labelCapacity = 15;
    labelLength = 0;
    reinterpret_cast<std::uint8_t*>(&label.field_0004)[0] = 0;
    AssignStringRange_429B60(
        &label, nullptr, const_cast<char*>(CSF_STR(0x00862978, "Size Y")), 6);

    sizeYMaximum = 2096;
    parameterId = 1;
    parameterCapacity = 15;
    parameterLength = 0;
    reinterpret_cast<std::uint8_t*>(&parameterName.field_0004)[0] = 0;
    AssignStringSubstring(&parameterName, nullptr, &label, 0, -1);
    parameterMinimum = 0;
    parameterMaximum = sizeYMaximum;
    parameterDefault = 200;
    GfxScreenEffects_AddParameter(parameters, nullptr, &parameterId);

    if (parameterCapacity >= 0x10 && parameterName.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(parameterName.field_0004));
    parameterCapacity = 15;
    parameterLength = 0;
    reinterpret_cast<std::uint8_t*>(&parameterName.field_0004)[0] = 0;

    if (labelCapacity >= 0x10 && label.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(label.field_0004));
    labelCapacity = 15;
    labelLength = 0;
    reinterpret_cast<std::uint8_t*>(&label.field_0004)[0] = 0;
    AssignStringRange_429B60(
        &label, nullptr, const_cast<char*>(CSF_STR(0x00862970, "Oscil")), 5);

    parameterId = 2;
    parameterCapacity = 15;
    parameterLength = 0;
    reinterpret_cast<std::uint8_t*>(&parameterName.field_0004)[0] = 0;
    AssignStringSubstring(&parameterName, nullptr, &label, 0, -1);
    parameterMinimum = 0;
    parameterMaximum = 100;
    parameterDefault = 0;
    GfxScreenEffects_AddParameter(parameters, nullptr, &parameterId);

    std::uint32_t result = parameterCapacity;
    if (result >= 0x10)
    {
        result = parameterName.field_0004;
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    parameterCapacity = 15;
    parameterLength = 0;
    reinterpret_cast<std::uint8_t*>(&parameterName.field_0004)[0] = 0;

    if (labelCapacity >= 0x10)
    {
        result = label.field_0004;
        if (result)
            return Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    return result;
}

HOOK(0x006F8DE0, C_008629f8_InitializeOscillationParameters, "auto");
