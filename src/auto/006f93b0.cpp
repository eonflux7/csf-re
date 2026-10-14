#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GfxScreenEffects_ResetEntryRange, 0x006F8810, (int begin, int end));
CSF_FUNC(std::uint32_t*, __fastcall, S_006f8ce0_AppendElement44, 0x006F8D50, (S_006f8ce0* self, void* , int* record));

static std::uint32_t __fastcall C_00862a80_InitializeFadeParameters(C_00862a80* self, void* /*edx*/)
{
    auto* entries = reinterpret_cast<S_006f8ce0*>(reinterpret_cast<std::uint8_t*>(self) + 0x50);
    if (CSF_AT(std::uint32_t, self, 0x54)) {
        GfxScreenEffects_ResetEntryRange(CSF_AT(int, self, 0x54), CSF_AT(int, self, 0x58));
        if (CSF_AT(std::uint32_t, self, 0x54))
            Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x54)));
    }
    CSF_AT(std::uint32_t, self, 0x54) = 0;
    CSF_AT(std::uint32_t, self, 0x58) = 0;
    CSF_AT(std::uint32_t, self, 0x5C) = 0;

    // Stack strings: +4 storage, +0x14 length, +0x18 capacity.
    // The record is an ID followed by the string and three limit/default words.
    std::uint32_t record[11];
    std::uint32_t source[10];

    source[6] = 15;
    source[5] = 0;
    *reinterpret_cast<std::uint8_t*>(&source[1]) = 0;
    AssignStringRange_429B60(source, nullptr, const_cast<char*>(CSF_STR(0x00862A58, "Duration")), 8);
    record[0] = 0;
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    AssignStringSubstring(record + 1, nullptr, source, 0, -1);
    record[8] = 1;
    record[9] = 100000;
    record[10] = 10000;
    S_006f8ce0_AppendElement44(entries, nullptr, reinterpret_cast<int*>(record));
    if (record[7] >= 16 && record[2])
        Mem_FreeDebug(reinterpret_cast<void*>(record[2]));
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    if (source[6] >= 16 && source[1])
        Mem_FreeDebug(reinterpret_cast<void*>(source[1]));

    source[6] = 15;
    source[5] = 0;
    *reinterpret_cast<std::uint8_t*>(&source[1]) = 0;
    AssignStringRange_429B60(source, nullptr, const_cast<char*>(CSF_STR(0x00862A50, "FadeIn")), 6);
    record[0] = 1;
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    AssignStringSubstring(record + 1, nullptr, source, 0, -1);
    record[8] = 0;
    record[9] = 1;
    record[10] = 1;
    S_006f8ce0_AppendElement44(entries, nullptr, reinterpret_cast<int*>(record));
    if (record[7] >= 16 && record[2])
        Mem_FreeDebug(reinterpret_cast<void*>(record[2]));
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    if (source[6] >= 16 && source[1])
        Mem_FreeDebug(reinterpret_cast<void*>(source[1]));

    source[6] = 15;
    source[5] = 0;
    *reinterpret_cast<std::uint8_t*>(&source[1]) = 0;
    AssignStringRange_429B60(source, nullptr, const_cast<char*>(CSF_STR(0x00862A48, "Reset")), 5);
    source[8] = 1;
    record[0] = 2;
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    AssignStringSubstring(record + 1, nullptr, source, 0, -1);
    record[8] = 0;
    record[9] = source[8];
    record[10] = 0;
    S_006f8ce0_AppendElement44(entries, nullptr, reinterpret_cast<int*>(record));
    if (record[7] >= 16 && record[2])
        Mem_FreeDebug(reinterpret_cast<void*>(record[2]));
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    if (source[6] >= 16 && source[1])
        Mem_FreeDebug(reinterpret_cast<void*>(source[1]));

    source[6] = 15;
    source[5] = 0;
    *reinterpret_cast<std::uint8_t*>(&source[1]) = 0;
    AssignStringRange_429B60(source, nullptr, const_cast<char*>(CSF_STR(0x008529A4, "Color")), 5);
    record[0] = 3;
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    AssignStringSubstring(record + 1, nullptr, source, 0, -1);
    record[8] = 0;
    record[9] = 0xFFFFFF;
    record[10] = 0xFFFFFF;
    S_006f8ce0_AppendElement44(entries, nullptr, reinterpret_cast<int*>(record));
    if (record[7] >= 16 && record[2])
        Mem_FreeDebug(reinterpret_cast<void*>(record[2]));
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    if (source[6] >= 16 && source[1])
        Mem_FreeDebug(reinterpret_cast<void*>(source[1]));

    source[6] = 15;
    source[5] = 0;
    *reinterpret_cast<std::uint8_t*>(&source[1]) = 0;
    AssignStringRange_429B60(source, nullptr, const_cast<char*>(CSF_STR(0x00862A40, "Rect")), 4);
    record[0] = 4;
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    AssignStringSubstring(record + 1, nullptr, source, 0, -1);
    record[8] = 0x80000000;
    record[9] = 0x7FFFFFFF;
    record[10] = 0;
    S_006f8ce0_AppendElement44(entries, nullptr, reinterpret_cast<int*>(record));
    if (record[7] >= 16 && record[2])
        Mem_FreeDebug(reinterpret_cast<void*>(record[2]));
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    if (source[6] >= 16 && source[1])
        Mem_FreeDebug(reinterpret_cast<void*>(source[1]));

    source[6] = 15;
    source[5] = 0;
    *reinterpret_cast<std::uint8_t*>(&source[1]) = 0;
    AssignStringRange_429B60(source, nullptr, const_cast<char*>(CSF_STR(0x00862A30, "TimeToFinish")), 12);
    record[0] = 5;
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    AssignStringSubstring(record + 1, nullptr, source, 0, -1);
    record[8] = 0;
    record[9] = 25500000;
    record[10] = 2550000;
    S_006f8ce0_AppendElement44(entries, nullptr, reinterpret_cast<int*>(record));
    if (record[7] >= 16 && record[2])
        Mem_FreeDebug(reinterpret_cast<void*>(record[2]));
    record[7] = 15;
    record[6] = 0;
    *reinterpret_cast<std::uint8_t*>(&record[2]) = 0;
    if (source[6] >= 16 && source[1])
        Mem_FreeDebug(reinterpret_cast<void*>(source[1]));

    self->field_0060 = 0;
    CSF_AT(std::uint32_t, self, 0x64) = 0;
    CSF_AT(std::uint32_t, self, 0x68) = 0;
    CSF_AT(std::uint32_t, self, 0x6C) = 0;
    CSF_AT(std::uint32_t, self, 0x70) = 0;
    return 0;
}

HOOK(0x006F93B0, C_00862a80_InitializeFadeParameters, "auto");
