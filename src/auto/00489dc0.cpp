#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __fastcall C_00833708_FindEntryByCachedObjectProvider(
    C_00833708* self, void* /*edx*/, int value)
{
    if (self->field_0014 == 0)
        return -1;

    uint32_t index = 0;
    uint32_t offset = 0;
    do {
        auto* entries = reinterpret_cast<uint8_t*>(self->field_000c);
        void* object = CSF_AT(void*, entries, offset);
        if (reinterpret_cast<uintptr_t>(GetCachedObjectProvider(object, nullptr)) ==
            static_cast<uint32_t>(value))
            return static_cast<int>(index);

        ++index;
        offset += 96;
    } while (index < self->field_0014);

    return -1;
}

HOOK(0x00489DC0, C_00833708_FindEntryByCachedObjectProvider, "auto");
