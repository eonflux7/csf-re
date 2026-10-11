#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __fastcall, CharActionFsm_GetPrimaryCapacity, 0x004769F0, (void* self, void* edx));
CSF_FUNC(uint64_t, __fastcall, GetItemCapacity, 0x00476A50, (void* self, void* edx));

static bool __fastcall C_008316d8_CanReload(C_008316d8* self, void* /*edx*/)
{
    if (!self->vSlot9C())
        return false;

    const uint32_t mode = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0040), 0xD4);
    int32_t capacity;
    int32_t count;

    switch (mode)
    {
    case 0:
        capacity = static_cast<int32_t>(GetItemCapacity(self, nullptr));
        count = static_cast<int32_t>(self->field_001c);
        break;
    case 1:
        capacity = static_cast<int32_t>(GetItemCapacity(self, nullptr));
        count = static_cast<int32_t>(self->field_0024);
        break;
    case 2:
        capacity = static_cast<int32_t>(CharActionFsm_GetPrimaryCapacity(self, nullptr));
        count = static_cast<int32_t>(self->field_0020);
        break;
    default:
        return false;
    }

    return count < capacity;
}

HOOK(0x00476E80, C_008316d8_CanReload, "auto");
