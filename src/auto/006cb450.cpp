#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall C_00860bb8_RemoveAndDestroyObject(
    C_00860bb8* self, void* /*edx*/, int objectAddress)
{
    if (!objectAddress)
        return;

    const uint32_t objectValue = static_cast<uint32_t>(objectAddress);
    const uint32_t firstCount = self->field_001c;
    if (firstCount)
    {
        auto* entries = reinterpret_cast<uint32_t*>(self->field_0014);
        uint32_t index = 0;
        while (entries[index] != objectValue)
        {
            ++index;
            if (index >= firstCount)
                break;
        }

        if (index < firstCount && index != static_cast<uint32_t>(-1))
        {
            entries = reinterpret_cast<uint32_t*>(self->field_0014);
            entries[index] = entries[self->field_001c - 1];
            --self->field_001c;
        }
    }

    const uint32_t secondCount = self->field_00d0;
    if (secondCount)
    {
        auto* entries = reinterpret_cast<uint32_t*>(self->field_00c8);
        uint32_t index = 0;
        while (entries[index] != objectValue)
        {
            ++index;
            if (index >= secondCount)
                break;
        }

        if (index < secondCount && index != static_cast<uint32_t>(-1))
        {
            entries = reinterpret_cast<uint32_t*>(self->field_00c8);
            entries[index] = entries[self->field_00d0 - 1];
            --self->field_00d0;
        }
    }

    void* object = reinterpret_cast<void*>(objectValue);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[1])(
        object, nullptr, 1);
}

HOOK(0x006CB450, C_00860bb8_RemoveAndDestroyObject, "auto");
