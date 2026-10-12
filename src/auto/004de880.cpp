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

// Callee bindings (by address)
CSF_FUNC(int, __fastcall, sub_4E0990, 0x004E0990, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees the per-entry sub-blocks and the entry table of S_004de880, then
// clears its counters. Returns the result of freeing the entry table
// (0 if there was no table).
static int __fastcall S_004de880_Reset(S_004de880* self, void* /*edx*/) {
    uint32_t bufferAtOffset30 = self->field_0030;
    uint32_t entryIndex = 0;

    self->field_0034 = 0;
    self->field_0038 = 0;
    if (bufferAtOffset30) {
        Mem_Free(reinterpret_cast<void*>(bufferAtOffset30));
        self->field_0030 = 0;
    }

    if (self->field_0028) {
        uint32_t byteOffset = 0;
        do {
            sub_4E0990(reinterpret_cast<void*>(self->field_0020 + byteOffset), nullptr);
            ++entryIndex;
            byteOffset += 0x40;
        } while (entryIndex < self->field_0028);
    }

    int result = static_cast<int>(self->field_0020);
    self->field_0024 = 0;
    self->field_0028 = 0;
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0020));
        self->field_0020 = 0;
    }
    return result;
}

HOOK(0x004DE880, S_004de880_Reset, "auto");
