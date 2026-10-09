#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_0046f900_ProcessEntry, 0x0046F900,
         (void* self, void* edx, int16_t* a2));

static uint32_t* __fastcall S_0046f900_ProcessAllEntries(
    S_0046f900* self, void* /*edx*/)
{
    // Retain the packet's address binding; no object vtable is accessed.
    (void)CSF_VTBL(0x0046F900);

    uint32_t begin = self->field_000c;
    uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0010 - begin) >> 3)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        int16_t* entry = CSF_AT(
            int16_t*, reinterpret_cast<void*>(self->field_000c),
            8 * index + 4);
        if (entry)
            S_0046f900_ProcessEntry(self, nullptr, entry);
    }

    begin = self->field_001c;
    count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0020 - begin) >> 3)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        S_0046f900_ProcessEntry(
            self, nullptr,
            reinterpret_cast<int16_t*>(self->field_001c + 8 * index));
    }

    begin = self->field_0030;
    uint32_t* result = reinterpret_cast<uint32_t*>(begin);
    count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0034 - begin) >> 3)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        result = S_0046f900_ProcessEntry(
            self, nullptr,
            reinterpret_cast<int16_t*>(self->field_0030 + 8 * index));
    }

    return result;
}

HOOK(0x0046FA20, S_0046f900_ProcessAllEntries, "auto");
