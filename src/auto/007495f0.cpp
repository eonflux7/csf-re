#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(char, __fastcall, SpawnPoint_ApplyFallbackSelection, 0x00749420, (void* self, void* edx, int arg0, void* a3, void* a2, int a11));
CSF_FUNC(char, __fastcall, SpawnPoint_CommitSelection, 0x00751110, (void* self, void* edx, char arg0, char a3, int a4, void** Block, int a6, int a7, int a8, int16_t a2, unsigned int a10, char a11));

// Uses a stack-resident 0x1C-byte string-range block, as the original does.
// Returns the callee's char result directly (al is passed through).
static char __fastcall C_00858180_ApplySpawnPointSelection(C_00858180* self, void* /*edx*/, int arg0, void* a3, void* sourceString, int a11)
{
    if (Sub_64B250(self, nullptr))
    {
        return SpawnPoint_ApplyFallbackSelection(self, nullptr, arg0, a3, sourceString, a11);
    }

    alignas(8) uint8_t stringBytes[0x1C] = {};
    uint8_t* stringBlock = stringBytes;
    CSF_AT(uint16_t, stringBlock, 4) = 0;
    CSF_AT(int, stringBlock, 0x14) = 0;
    CSF_AT(unsigned int, stringBlock, 0x18) = 7;

    AssignStringRange_488830(stringBlock, nullptr, sourceString, 0u, 0xFFFFFFFFu);

    return SpawnPoint_CommitSelection(
        self, nullptr,
        static_cast<char>(arg0),
        static_cast<char>(reinterpret_cast<uintptr_t>(a3)),
        CSF_AT(int, stringBlock, 0x00),
        CSF_AT(void**, stringBlock, 0x04),
        CSF_AT(int, stringBlock, 0x08),
        CSF_AT(int, stringBlock, 0x0C),
        CSF_AT(int, stringBlock, 0x10),
        static_cast<int16_t>(CSF_AT(int, stringBlock, 0x14)),
        CSF_AT(unsigned int, stringBlock, 0x18),
        static_cast<char>(a11));
}

HOOK(0x007495F0, C_00858180_ApplySpawnPointSelection, "auto");
