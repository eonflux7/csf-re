#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Replaces the record attached to a TAiState (this+0x0C). A nonzero payload
// allocates a fresh 20-byte record linked back to the state; zero just clears the link.
static void __fastcall TAiState_ReplaceAttachedRecord(TAiState* self, void* /*edx*/, int payload)
{
    if (payload != 0)
    {
        uint32_t* record = reinterpret_cast<uint32_t*>(
            static_cast<std::uintptr_t>(CoreMemory_AllocUntagged(20)));
        if (record != nullptr)
        {
            *reinterpret_cast<void***>(record) = CSF_VTBL(0x0083DEF8);
            record[1] = 0;
            record[2] = 0;
            record[3] = 0;
            record[4] = 0;
        }
        else
        {
            record = nullptr;
        }

        uint32_t stateField4 = CSF_AT(uint32_t, self, 0x4);
        record[3] = 0;
        record[2] = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(self));
        record[4] = static_cast<uint32_t>(payload);
        record[1] = stateField4;
        CSF_AT(uint32_t, self, 0xC) = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(record));
    }
    else
    {
        CSF_AT(uint32_t, self, 0xC) = 0;
    }
}

HOOK(0x4DBEB0, TAiState_ReplaceAttachedRecord, "auto");
