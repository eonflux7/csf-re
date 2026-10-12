#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));

static int __fastcall C_00833708_FindSubObjectByField74(
    C_00833708* self, void* /*edx*/, int value)
{
    if (self->field_0014 == 0)
        return -1;

    uint32_t index = 0;
    uint32_t offset = 0;
    do {
        auto* entries = reinterpret_cast<uint8_t*>(self->field_000c);
        auto* object = CSF_AT(C_008319c0*, entries, offset);
        if (C_008319c0_GetSubObjectField74(object, nullptr) == value)
            return static_cast<int>(index);

        ++index;
        offset += 96;
    } while (index < self->field_0014);

    return -1;
}

HOOK(0x00489D70, C_00833708_FindSubObjectByField74, "auto");
