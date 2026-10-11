#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __fastcall, GetItemCapacity, 0x00476A50, (void* self, void* edx));

static uint64_t __fastcall C_008316d8_AddAmmoClamped(
    C_008316d8* self, void* /*edx*/, int amount)
{
    self->field_001c += static_cast<uint32_t>(amount);
    uint64_t capacity = GetItemCapacity(self, nullptr);
    if (static_cast<int32_t>(self->field_001c) >
        static_cast<int32_t>(static_cast<uint32_t>(capacity)))
    {
        capacity = GetItemCapacity(self, nullptr);
        self->field_001c = static_cast<uint32_t>(capacity);
    }
    return capacity;
}

HOOK(0x00476FA0, C_008316d8_AddAmmoClamped, "auto");
