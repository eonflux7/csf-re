#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00864508_IsObjectListed(
    C_00864508* self, void* /*edx*/, int object)
{
    if (object == 0)
        return 0;

    const uint32_t recordsBegin = self->field_0014;
    const uint32_t recordCount = recordsBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0018 - recordsBegin) >> 6)
        : 0;

    uint32_t recordIndex = 0;
    if (recordCount != 0)
    {
        uint32_t recordAddress = self->field_0014 + 0x38;
        do
        {
            if (CSF_AT(uint32_t, reinterpret_cast<void*>(recordAddress), 0) ==
                static_cast<uint32_t>(object))
                break;

            ++recordIndex;
            recordAddress += 0x40;
        } while (recordIndex < recordCount);
    }

    if (recordIndex == recordCount)
        return 0;

    const uint32_t indicesBegin = self->field_0024;
    const uint32_t indexCount = indicesBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0028 - indicesBegin) >> 2)
        : 0;

    uint32_t listIndex = 0;
    if (indexCount != 0)
    {
        uint32_t indexAddress = self->field_0024;
        do
        {
            if (CSF_AT(uint32_t, reinterpret_cast<void*>(indexAddress), 0) ==
                recordIndex)
                break;

            ++listIndex;
            indexAddress += 4;
        } while (listIndex < indexCount);
    }

    return listIndex < indexCount ? 1 : 0;
}

HOOK(0x0071A860, C_00864508_IsObjectListed, "auto");
