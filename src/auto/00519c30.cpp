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

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Walks the 16-byte entry table between field_000c (begin) and field_0010 (end).
// Each entry's pointer at +4 is freed and the entry's +4/+8/+C words are cleared.
static void __fastcall SceneLoader_PrepareLoadInterface(C_00840724* self, void* /*edx*/)
{
    uint32_t entryIndex = 0;
    for (uint32_t offset = 0;; offset += 16)
    {
        uint32_t tableBegin = self->field_000c;
        if (tableBegin == 0)
            break;

        // Signed element count: (end - begin) >> 4, compared unsigned against the index.
        int32_t entryCount = static_cast<int32_t>(self->field_0010 - tableBegin) >> 4;
        if (entryIndex >= static_cast<uint32_t>(entryCount))
            break;

        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(tableBegin) + offset);
        uint32_t blockWord = CSF_AT(uint32_t, entry, 4);
        if (blockWord != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(blockWord));

        CSF_AT(uint32_t, entry, 4) = 0;
        CSF_AT(uint32_t, entry, 8) = 0;
        CSF_AT(uint32_t, entry, 12) = 0;

        ++entryIndex;
    }
}

HOOK(0x00519C30, SceneLoader_PrepareLoadInterface, "auto");
