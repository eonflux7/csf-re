#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Callee: returns an entry index for a new position record (signature from packet, name chosen here).
CSF_FUNC(int, __fastcall, S_00443a00_AcquireEntryIndex, 0x00443A00, (void* self, void* edx, int a2));

// Reserves an entry for a position and clears its flag dword at +0x24 (40-byte records).
static int __fastcall S_00443a00_RegisterPosition(S_00443a00* self, void* /*edx*/, int a2)
{
    int index = S_00443a00_AcquireEntryIndex(self, nullptr, a2);

    uint8_t* entryBase = reinterpret_cast<uint8_t*>(self->field_000c);
    CSF_AT(int, entryBase, 40 * index + 36) = 0;

    return index;
}

HOOK(0x00443B50, S_00443a00_RegisterPosition, "auto");
