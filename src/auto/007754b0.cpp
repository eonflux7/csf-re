#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bound by address: game heap debug free, cdecl, one argument.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Record layout (44 bytes). The original addresses each record through a
// pointer at record+32 (esi), so offsets below are relative to that pointer:
//   esi+0    owned buffer pointer, freed and cleared (record+32)
//   esi+4    cleared (record+36)
//   esi+8    cleared (record+40)
//   esi-8    capacity (record+24), compared unsigned against 0x10; reset to 15
//   esi-12   length/count (record+20), reset to 0
//   esi-28   owned buffer pointer (record+4), freed when capacity >= 0x10;
//            only its low byte is cleared here
static uint8_t* RecordFromAddress(int address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(address)));
}

static void* PointerFromWord(uint32_t word)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(word));
}

static int __cdecl UpdateGameDataRecordLinks(int firstRecord, int endRecord, int /*a3*/, int /*a4*/)
{
    if (firstRecord != endRecord)
    {
        int cursor = firstRecord;
        do
        {
            uint8_t* esi = RecordFromAddress(cursor + 0x20);

            // Release the pointer at esi+0 if set, then clear esi+0, esi+4, esi+8.
            uint32_t primaryBuffer = CSF_AT(uint32_t, esi, 0);
            if (primaryBuffer != 0)
                Mem_FreeDebug(PointerFromWord(primaryBuffer));
            CSF_AT(uint32_t, esi, 0) = 0;
            CSF_AT(uint32_t, esi, 4) = 0;
            CSF_AT(uint32_t, esi, 8) = 0;

            // Release the buffer at esi-28 only when esi-8 is at least 0x10 (unsigned).
            if (CSF_AT(uint32_t, esi, -8) >= 0x10u)
            {
                uint32_t secondaryBuffer = CSF_AT(uint32_t, esi, -28);
                if (secondaryBuffer != 0)
                    Mem_FreeDebug(PointerFromWord(secondaryBuffer));
            }

            CSF_AT(uint32_t, esi, -8) = 15;
            CSF_AT(uint32_t, esi, -12) = 0;
            CSF_AT(uint8_t, esi, -28) = 0;

            cursor += 44;
        } while (cursor != endRecord);
    }

    // The original leaves eax holding a2 on every exit path.
    return endRecord;
}

HOOK(0x007754B0, UpdateGameDataRecordLinks, "auto");
