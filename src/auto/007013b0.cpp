#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall S_007013b0_RemoveCachedEntry(
    S_007013b0* self, void* /*edx*/, void* entry)
{
    uint32_t begin = self->field_0018;
    const int32_t index = CSF_AT(int32_t, entry, 0x04);
    int32_t result = 0;
    bool removeLast = false;

    if (begin) {
        result = static_cast<int32_t>(self->field_001c - begin) >> 2;
        removeLast = result == 1;
    }

    if (!removeLast) {
        begin = self->field_0018;
        result = begin
            ? (static_cast<int32_t>(self->field_001c - begin) >> 2)
            : 0;
        result = static_cast<int32_t>(static_cast<uint32_t>(result) - 1);
        removeLast = result == index;
    }

    if (!removeLast) {
        begin = self->field_0018;
        const int32_t count = begin
            ? (static_cast<int32_t>(self->field_001c - begin) >> 2)
            : 0;
        const uint32_t lastOffset = 4 * static_cast<uint32_t>(count) - 4;

        const uint32_t lastId = CSF_AT(
            uint32_t,
            reinterpret_cast<void*>(self->field_0018 + lastOffset),
            0);
        void* movedEntry = reinterpret_cast<void*>(
            self->field_0008 + (lastId << 4));
        CSF_AT(int32_t, movedEntry, 0x04) = index;

        begin = self->field_0018;
        result = static_cast<int32_t>(begin);
        const uint32_t movedId = CSF_AT(
            uint32_t, reinterpret_cast<void*>(begin + lastOffset), 0);
        CSF_AT(
            uint32_t,
            reinterpret_cast<void*>(begin + 4 * static_cast<uint32_t>(index)),
            0) = movedId;
    }

    begin = self->field_0018;
    if (begin) {
        const uint32_t end = self->field_001c;
        result = static_cast<int32_t>(end - begin) >> 2;
        if (result)
            self->field_001c = end - 4;
    }

    CSF_AT(int32_t, entry, 0x04) = -1;
    return result;
}

HOOK_KEEP_ECX(0x007013B0, S_007013b0_RemoveCachedEntry, 1, "gfx_particle_system_defs");
