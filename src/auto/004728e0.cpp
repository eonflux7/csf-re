#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(bool, __fastcall, MatchesEntry, 0x004725F0,
                (void* self, void* edx, int a2));

static int __fastcall S_00472350_FindMatchingEntry(
    S_00472350* self, void* /*edx*/, int key)
{
    (void)CSF_VTBL(0x004725F0);

    const uint32_t begin = self->field_0010;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0014 - begin) >> 2)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        void* entry = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0010), index * 4);
        if (MatchesEntry(entry, nullptr, key))
            return static_cast<int32_t>(index);
    }
    return -1;
}

HOOK(0x004728E0, S_00472350_FindMatchingEntry, "auto");
