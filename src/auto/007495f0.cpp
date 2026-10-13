#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SpawnPoint_ApplyFallbackSelection, 0x00749420, (void* self, void* edx, int arg0, void* a3, void* a2, int a11));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(char, __fastcall, SpawnPoint_CommitSelection, 0x00751110, (void* self, void* edx, char arg0, char a3, int a4, void** Block, int a6, int a7, int a8, int16_t a2, unsigned int a10, char a11));

static char __fastcall C_00858180_ApplySpawnPointSelection(C_00858180* self, void* /*edx*/, char arg0, char a3, void* a2, int a11)
{
    if (Sub_64B250(self, nullptr))
    {
        return SpawnPoint_ApplyFallbackSelection(self, nullptr,
            static_cast<int>(arg0),
            reinterpret_cast<void*>(static_cast<std::intptr_t>(a3)),
            a2,
            a11);
    }

    // Temporary string-range block that the original builds on the stack (0x1C bytes).
    alignas(8) std::uint8_t stringBlock[0x1C];
    CSF_AT(std::uint32_t, stringBlock, 0x18) = 7;
    CSF_AT(std::uint32_t, stringBlock, 0x14) = 0;
    CSF_AT(std::uint16_t, stringBlock, 0x04) = 0;

    AssignStringRange_488830(stringBlock, nullptr, a2, 0, 0xFFFFFFFF);

    // The callee receives the block contents as its stack arguments, followed by a11.
    return SpawnPoint_CommitSelection(self, nullptr,
        arg0,
        a3,
        CSF_AT(int, stringBlock, 0x00),
        CSF_AT(void**, stringBlock, 0x04),
        CSF_AT(int, stringBlock, 0x08),
        CSF_AT(int, stringBlock, 0x0C),
        CSF_AT(int, stringBlock, 0x10),
        CSF_AT(int16_t, stringBlock, 0x14),
        CSF_AT(unsigned int, stringBlock, 0x18),
        static_cast<char>(a11));
}

HOOK(0x007495F0, C_00858180_ApplySpawnPointSelection, "auto");
