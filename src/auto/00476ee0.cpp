#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __fastcall, CharActionFsm_GetPrimaryCapacity, 0x004769F0, (void* self, void* edx));
CSF_FUNC(uint64_t, __fastcall, GetItemCapacity, 0x00476A50, (void* self, void* edx));

static int __fastcall C_008316d8_AddAmmunition(
    C_008316d8* self, void* /*edx*/, uint32_t* source)
{
    uint32_t firstAmount;
    uint32_t thirdAmount;
    uint32_t secondAmount;

    if (source)
    {
        firstAmount = CSF_AT(uint32_t, source, 0x188);
        thirdAmount = CSF_AT(uint32_t, source, 0x190);
        secondAmount = CSF_AT(uint32_t, source, 0x18C);
    }
    else
    {
        void* definition = reinterpret_cast<void*>(self->field_0040);
        firstAmount = CSF_AT(uint32_t, definition, 0x104);
        thirdAmount = CSF_AT(uint32_t, definition, 0x110);
        secondAmount = CSF_AT(uint32_t, definition, 0x108);
    }

    int result = static_cast<int32_t>(secondAmount);
    switch (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0040), 0xD4))
    {
    case 0:
        self->field_001c += firstAmount;
        result = static_cast<int32_t>(
            static_cast<uint32_t>(GetItemCapacity(self, nullptr)));
        if (static_cast<int32_t>(self->field_001c) > result)
        {
            result = static_cast<int32_t>(
                static_cast<uint32_t>(GetItemCapacity(self, nullptr)));
            self->field_001c = static_cast<uint32_t>(result);
        }
        break;

    case 1:
        self->field_0024 += thirdAmount;
        result = static_cast<int32_t>(
            static_cast<uint32_t>(GetItemCapacity(self, nullptr)));
        if (static_cast<int32_t>(self->field_0024) > result)
        {
            result = static_cast<int32_t>(
                static_cast<uint32_t>(GetItemCapacity(self, nullptr)));
            self->field_0024 = static_cast<uint32_t>(result);
        }
        break;

    case 2:
        self->field_0020 += secondAmount;
        result = static_cast<int32_t>(
            static_cast<uint32_t>(CharActionFsm_GetPrimaryCapacity(self, nullptr)));
        if (static_cast<int32_t>(self->field_0020) > result)
        {
            result = static_cast<int32_t>(
                static_cast<uint32_t>(CharActionFsm_GetPrimaryCapacity(self, nullptr)));
            self->field_0020 = static_cast<uint32_t>(result);
        }
        break;
    }

    return result;
}

HOOK(0x00476EE0, C_008316d8_AddAmmunition, "auto");
