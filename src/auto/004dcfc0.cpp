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

CSF_FUNC(void, __fastcall, NodeList_Clear, 0x004DC810, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_0086c288_ResetGraphState, 0x0075D1F0, (C_0086c288* self, void*));

// Resets the graph state, clears the three embedded lists, frees every
// non-null per-entry buffer pointer (at entry offset +0xC, 16-byte entries),
// then frees the entry array itself and zeroes the bookkeeping fields.
static int __fastcall C_0083e0d8_ClearNodeStorage(C_0083e0d8* self, void* /*edx*/)
{
    C_0086c288_ResetGraphState(self, nullptr);

    NodeList_Clear(&self->field_0034, nullptr);
    NodeList_Clear(&self->field_0044, nullptr);
    NodeList_Clear(&self->field_0054, nullptr);

    // Entry count = (end - begin) / 16, signed shift; zero if the array is absent.
    int32_t entryCount = 0;
    if (self->field_0068 != 0)
        entryCount = static_cast<int32_t>(self->field_006c - self->field_0068) >> 4;

    if (entryCount != 0) {
        uint32_t remaining = static_cast<uint32_t>(entryCount);
        uint32_t offset = 0;
        do {
            uint8_t* entries = reinterpret_cast<uint8_t*>(self->field_0068);
            uint32_t buffer = CSF_AT(uint32_t, entries, offset + 0xC);
            if (buffer != 0) {
                Mem_Free(reinterpret_cast<void*>(buffer));
                entries = reinterpret_cast<uint8_t*>(self->field_0068);
                CSF_AT(uint32_t, entries, offset + 0xC) = 0;
            }
            offset += 0x10;
            --remaining;
        } while (remaining != 0);
    }

    int result = static_cast<int>(self->field_0068);
    if (result != 0)
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0068));

    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    return result;
}

HOOK(0x004DCFC0, C_0083e0d8_ClearNodeStorage, "auto");
