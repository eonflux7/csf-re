#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static void __fastcall C_008655e8_RemoveEntry(
    C_008655e8* self, void* /*edx*/, int entry)
{
    if (entry == 0)
        return;

    const uint32_t originalCount = self->field_003c;
    uint32_t index = 0;
    if (originalCount == 0)
        return;

    auto* cursor = reinterpret_cast<uint32_t*>(self->field_0034);
    while (*cursor != static_cast<uint32_t>(entry))
    {
        ++index;
        ++cursor;
        if (index >= originalCount)
            return;
    }

    const uint32_t newCount = self->field_003c - 1u;
    self->field_003c = newCount;
    if (index == newCount)
        return;

    auto* destination =
        reinterpret_cast<uint8_t*>(self->field_0034) + index * 4u;
    _memcpy(destination, destination + 4u, (newCount - index) * 4u);
}

HOOK(0x00726DB0, C_008655e8_RemoveEntry, "auto");
