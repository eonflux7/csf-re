#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GfxScreenEffects_ResetEntryRange, 0x006F8810, (int begin, int end));
CSF_FUNC(uint32_t*, __fastcall, GfxScreenEffects_AppendParameter, 0x006F8D50, (void* self, void* edx, int* a3));

static inline __attribute__((always_inline)) void GfxScreenEffects_InitializeTemporaryString(
    C_0082d410& text, std::uint32_t& length, std::uint32_t& capacity)
{
    capacity = 15;
    length = 0;
    reinterpret_cast<std::uint8_t*>(&text.field_0004)[0] = 0;
}

static inline __attribute__((always_inline)) void GfxScreenEffects_FreeTemporaryString(
    C_0082d410& text, const std::uint32_t& capacity)
{
    if (capacity >= 0x10 && text.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(text.field_0004));
}

static inline __attribute__((always_inline)) void GfxScreenEffects_AddDefaultParameter(
    void* parameters, std::uint32_t* frame,
    char* text, unsigned int length, int index,
    std::uint32_t minimum, int maximum, int defaultValue)
{
    // The original frame contains an index, a 0x28-byte parameter,
    // and a 0x28-byte temporary string.
    auto* entry = reinterpret_cast<int*>(frame);
    auto& name = *reinterpret_cast<C_0082d410*>(frame + 1);
    auto& source = *reinterpret_cast<C_0082d410*>(frame + 11);

    std::uint32_t& nameLength = frame[6];
    std::uint32_t& nameCapacity = frame[7];
    std::uint32_t& sourceLength = frame[16];
    std::uint32_t& sourceCapacity = frame[17];
    int& resetMaximum = reinterpret_cast<int*>(frame)[19];

    GfxScreenEffects_InitializeTemporaryString(
        source, sourceLength, sourceCapacity);
    AssignStringRange_429B60(&source, nullptr, text, length);

    if (index == 2)
        resetMaximum = 1;

    *entry = index;
    GfxScreenEffects_InitializeTemporaryString(
        name, nameLength, nameCapacity);
    AssignStringSubstring(&name, nullptr, &source, 0, -1);

    frame[8] = minimum;
    reinterpret_cast<int*>(frame)[9] =
        index == 2 ? resetMaximum : maximum;
    reinterpret_cast<int*>(frame)[10] = defaultValue;
    GfxScreenEffects_AppendParameter(parameters, nullptr, entry);

    GfxScreenEffects_FreeTemporaryString(name, nameCapacity);
    GfxScreenEffects_InitializeTemporaryString(
        name, nameLength, nameCapacity);
    GfxScreenEffects_FreeTemporaryString(source, sourceCapacity);
}

static int __fastcall C_00862a80_InitializeFadeParameters(
    C_00862a80* self, void* /*edx*/)
{
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

    alignas(8) std::uint8_t temporaryBytes[0x54];
    auto* frame = reinterpret_cast<std::uint32_t*>(temporaryBytes);

    GfxScreenEffects_AddDefaultParameter(
        parameters, frame,
        const_cast<char*>(CSF_STR(0x00862A58, "Duration")),
        8, 0, 1, 100000, 10000);
    GfxScreenEffects_AddDefaultParameter(
        parameters, frame,
        const_cast<char*>(CSF_STR(0x00862A50, "FadeIn")),
        6, 1, 0, 1, 1);
    GfxScreenEffects_AddDefaultParameter(
        parameters, frame,
        const_cast<char*>(CSF_STR(0x00862A48, "Reset")),
        5, 2, 0, 1, 0);
    GfxScreenEffects_AddDefaultParameter(
        parameters, frame,
        const_cast<char*>(CSF_STR(0x008529A4, "Color")),
        5, 3, 0, 0xFFFFFF, 0xFFFFFF);
    GfxScreenEffects_AddDefaultParameter(
        parameters, frame,
        const_cast<char*>(CSF_STR(0x00862A40, "Rect")),
        4, 4, 0x80000000, 0x7FFFFFFF, 0);
    GfxScreenEffects_AddDefaultParameter(
        parameters, frame,
        const_cast<char*>(CSF_STR(0x00862A30, "TimeToFinish")),
        12, 5, 0, 25500000, 2550000);

    self->field_0060 = 0;
    CSF_AT(std::uint32_t, self, 0x64) = 0;
    CSF_AT(std::uint32_t, self, 0x68) = 0;
    CSF_AT(std::uint8_t, self, 0x6C) = 0;
    CSF_AT(std::uint32_t, self, 0x70) = 0;
    return 0;
}

HOOK(0x006F93B0, C_00862a80_InitializeFadeParameters, "auto");
