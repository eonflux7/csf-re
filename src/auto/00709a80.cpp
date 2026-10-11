#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x70CC60: __thiscall-style method, self passed in ecx, edx unused.
CSF_FUNC(int, __fastcall, WaterEntry_Process, 0x0070CC60, (void* self, void* edx));

// int __cdecl sub_709A80(int a1, int a2, int a3, int a4)
// Walks [a1, a2) in steps of 8 bytes, calling WaterEntry_Process on each address.
// Returns the value of the last call. a3/a4 are pushed by the caller but unused here.
static int __cdecl Water_ProcessEntryRange(int rangeBegin, int rangeEnd, int unusedArg3, int unusedArg4) {
    (void)unusedArg3;
    (void)unusedArg4;

    int result = 0;
    uint32_t entryAddress = static_cast<uint32_t>(rangeBegin);
    const uint32_t entryEnd = static_cast<uint32_t>(rangeEnd);

    while (entryAddress != entryEnd) {
        result = WaterEntry_Process(reinterpret_cast<void*>(entryAddress), nullptr);
        entryAddress += 8;
    }
    return result;
}

HOOK(0x709A80, Water_ProcessEntryRange, "auto");
