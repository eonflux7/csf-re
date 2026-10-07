#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint8_t __fastcall C_00864508_DoAllReferencedObjectsPassCheck(
    C_00864508* self, void* /*edx*/)
{
    uint8_t allPass = 1;

    const uint32_t primaryBegin = self->field_0024;
    const uint32_t primaryCount = primaryBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0028 - primaryBegin) >> 2)
        : 0;

    for (uint32_t index = 0; index < primaryCount && allPass; ++index)
    {
        const uint32_t recordIndex = CSF_AT(
            uint32_t,
            reinterpret_cast<void*>(self->field_0024 + index * 4u),
            0);

        if (recordIndex != UINT32_MAX)
        {
            void* object = CSF_AT(
                void*,
                reinterpret_cast<void*>(
                    self->field_0014 + (recordIndex << 6)),
                0x38);

            if (object)
            {
                const uint8_t result =
                    ((uint8_t(__fastcall*)(void*, void*))
                         CSF_AT(void**, object, 0)[0x2C / 4])(
                        object, nullptr);

                if (!result)
                    allPass = 0;
            }
        }
    }

    const uint32_t secondaryBegin = self->field_0034;
    const uint32_t secondaryCount = secondaryBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0038 - secondaryBegin) >> 3)
        : 0;

    for (uint32_t index = 0; index < secondaryCount && allPass; ++index)
    {
        const uint32_t objectIndex = CSF_AT(
            uint32_t,
            reinterpret_cast<void*>(self->field_0034 + index * 8u),
            0);

        if (objectIndex != UINT32_MAX)
        {
            void* object = CSF_AT(
                void*,
                reinterpret_cast<void*>(self->field_0048 + objectIndex * 4u),
                0);

            if (object)
            {
                const uint8_t result =
                    ((uint8_t(__fastcall*)(void*, void*))
                         CSF_AT(void**, object, 0)[0x2C / 4])(
                        object, nullptr);

                if (!result)
                    allPass = 0;
            }
        }
    }

    return allPass;
}

HOOK(0x0071A940, C_00864508_DoAllReferencedObjectsPassCheck, "auto");
