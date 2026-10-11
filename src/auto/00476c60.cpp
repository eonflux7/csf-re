#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __fastcall, CharActionFsm_GetPrimaryCapacity, 0x004769F0, (void* self, void* edx));
CSF_FUNC(uint64_t, __fastcall, GetItemCapacity, 0x00476A50, (void* self, void* edx));

static int __fastcall C_008316d8_SetAmmunition(
    C_008316d8* self, void* /*edx*/, int primaryAmmo, int secondaryAmmo)
{
    if (!self->vSlot9C())
        return 0;

    int result = static_cast<int32_t>(self->field_0040);
    void* definition = reinterpret_cast<void*>(self->field_0040);

    switch (CSF_AT(int32_t, definition, 0xD4))
    {
    case 0:
        result = CSF_AT(int32_t, definition, 0x104);
        self->field_001c = primaryAmmo > result ? result : primaryAmmo;
        break;

    case 1:
    {
        int capacity = CSF_AT(int32_t, definition, 0x104);
        if (primaryAmmo <= capacity)
        {
            self->field_001c = primaryAmmo;
            self->field_0024 = 0;
        }
        else
        {
            self->field_001c = capacity;
            int remaining = static_cast<int32_t>(
                static_cast<uint32_t>(primaryAmmo) -
                CSF_AT(uint32_t, definition, 0x104));
            if (remaining > 0)
            {
                result = static_cast<int32_t>(
                    GetItemCapacity(self, nullptr));
                if (remaining > result)
                {
                    result = static_cast<int32_t>(
                        GetItemCapacity(self, nullptr));
                    remaining = result;
                }
                self->field_0024 = remaining;
            }
        }
        break;
    }

    case 2:
    {
        int capacity = CSF_AT(int32_t, definition, 0x104);
        self->field_001c = primaryAmmo > capacity ? capacity : primaryAmmo;

        result = static_cast<int32_t>(
            CharActionFsm_GetPrimaryCapacity(self, nullptr));
        int ammunition = secondaryAmmo;
        if (secondaryAmmo > result)
        {
            result = static_cast<int32_t>(
                CharActionFsm_GetPrimaryCapacity(self, nullptr));
            ammunition = result;
        }
        self->field_0020 = ammunition;
        break;
    }

    default:
        break;
    }

    return result;
}

HOOK(0x00476C60, C_008316d8_SetAmmunition, "auto");
