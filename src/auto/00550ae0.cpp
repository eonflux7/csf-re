#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

// Clears the spawn-entry table of the MP game-mode object: two 16-bit counters
// and one dword at +0x700..+0x70B, then 32 records of 0x38 bytes each starting at +0x00.
// Each record gets its text-input subobject (at record+4) initialized and several fields zeroed.
static int __fastcall ResetSpawnEntries(uint8_t* self, void* /*edx*/)
{
    CSF_AT(uint16_t, self, 0x700) = 0;
    CSF_AT(uint16_t, self, 0x702) = 0;
    CSF_AT(uint32_t, self, 0x708) = 0;

    uint32_t recordOffset = 0;
    int remaining = 32;
    do
    {
        uint8_t* record = self + recordOffset;

        CSF_AT(uint32_t, record, 0x00) = 0;
        TextInputSubobject_Init(record + 0x04, nullptr, &g_byte_82E65C);

        CSF_AT(uint32_t, record, 0x20) = 0;
        CSF_AT(uint32_t, record, 0x24) = 0;
        CSF_AT(uint32_t, record, 0x28) = 0;
        CSF_AT(uint32_t, record, 0x34) = 0;

        recordOffset += 0x38;
        --remaining;
    } while (remaining != 0);

    return 0;
}

HOOK(0x00550AE0, ResetSpawnEntries, "auto");
