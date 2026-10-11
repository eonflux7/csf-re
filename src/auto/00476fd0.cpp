#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __fastcall, CharActionFsm_GetPrimaryCapacity, 0x004769F0, (void* self, void* edx));
CSF_FUNC(uint64_t, __fastcall, GetItemCapacity, 0x00476A50, (void* self, void* edx));

static char __fastcall C_008316d8_FillAmmoToCapacity(C_008316d8* self, void* /*edx*/)
{
    const int mode = CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0xD4);
    switch (mode)
    {
    case 0:
    {
        const int32_t capacity = static_cast<int32_t>(GetItemCapacity(self, nullptr));
        if (static_cast<int32_t>(self->field_001c) < capacity)
        {
            self->field_001c = capacity;
            return 1;
        }
        break;
    }
    case 1:
    {
        const int32_t capacity = static_cast<int32_t>(GetItemCapacity(self, nullptr));
        if (static_cast<int32_t>(self->field_0024) < capacity)
        {
            self->field_0024 = capacity;
            return 1;
        }
        break;
    }
    case 2:
    {
        const int32_t capacity = static_cast<int32_t>(CharActionFsm_GetPrimaryCapacity(self, nullptr));
        if (static_cast<int32_t>(self->field_0020) < capacity)
        {
            self->field_0020 = capacity;
            return 1;
        }
        break;
    }
    }
    return 0;
}

HOOK(0x00476FD0, C_008316d8_FillAmmoToCapacity, "auto");
