#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00446340_GrowCapacity, 0x00446340,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SerializeEntry, 0x00447370,
         (void* self, void* edx, void* a2));

static uint32_t __stdcall S_00446340_SerializeEntries(
    S_00446340* self, S_00691860* stream)
{
    // The packet also identifies the growth routine's address as a vtable.
    (void)CSF_VTBL(0x00446340);

    const uint8_t savedMode = CSF_AT(uint8_t, stream, 0x15);
    const bool useExistingCount = CSF_AT(uint8_t, stream, 0x14) != 0;
    const uint8_t previousError = CSF_AT(uint8_t, stream, 0x00);

    alignas(8) uint8_t countBytes[0x4] = {};
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);
    if (useExistingCount)
        count = self->field_0008;

    uint32_t result = 0;
    if (previousError == 0)
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        result = !((bool(__fastcall*)(void*, void*, uint32_t*, void*))
                       CSF_AT(void**, backend, 0)[0x78 / 4])(
                           backend, nullptr, &count, stream);
    }

    CSF_AT(uint8_t, stream, 0x00) = static_cast<uint8_t>(result);
    if (result != 0)
    {
        CSF_AT(uint8_t, stream, 0x00) = 1;
        return result;
    }

    if (useExistingCount || savedMode != 0)
    {
        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < count)
        {
            void* entry =
                reinterpret_cast<uint8_t*>(self->field_0000) + offset;
            const bool canSerialize = CSF_AT(uint8_t, stream, 0x00) == 0;
            CSF_AT(uint32_t, stream, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, stream, 0x15) = savedMode;
                CSF_AT(uint8_t, stream, 0x16) = 0;
                SerializeEntry(entry, nullptr, stream);
            }
            result = ++index;
            offset += 36;
        }
    }
    else
    {
        result = self->field_0004;
        self->field_0008 = 0;
        if (count > result)
        {
            result = reinterpret_cast<uint32_t>(
                S_00446340_GrowCapacity(
                    self, nullptr, static_cast<int>(count - result)));
        }

        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < count)
        {
            if (self->field_0008 >= self->field_0004)
            {
                S_00446340_GrowCapacity(
                    self, nullptr, static_cast<int>(self->field_000c));
            }

            const uint32_t entryIndex = self->field_0008;
            void* newEntry =
                reinterpret_cast<uint8_t*>(self->field_0000) + 36 * entryIndex;
            self->field_0008 = entryIndex + 1;

            CSF_AT(int32_t, newEntry, 0x00) = -1;
            CSF_AT(int32_t, newEntry, 0x08) = -1;
            CSF_AT(int32_t, newEntry, 0x0C) = -1;
            CSF_AT(uint32_t, newEntry, 0x10) = 0;
            CSF_AT(uint32_t, newEntry, 0x14) = 0;
            CSF_AT(uint32_t, newEntry, 0x18) = 0;
            CSF_AT(uint32_t, newEntry, 0x1C) = 0;
            CSF_AT(float, newEntry, 0x20) = 1.0f;

            void* entry =
                reinterpret_cast<uint8_t*>(self->field_0000) + offset;
            const bool canSerialize = CSF_AT(uint8_t, stream, 0x00) == 0;
            CSF_AT(uint32_t, stream, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, stream, 0x15) = 0;
                CSF_AT(uint8_t, stream, 0x16) = 0;
                SerializeEntry(entry, nullptr, stream);
            }

            result = ++index;
            offset += 36;
        }
    }

    uint8_t error = 1;
    if (CSF_AT(uint8_t, stream, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        result = ((bool(__fastcall*)(void*, void*, void*))
                      CSF_AT(void**, backend, 0)[0x7C / 4])(
                          backend, nullptr, stream);
        if (result != 0)
            error = 0;
    }
    CSF_AT(uint8_t, stream, 0x00) = error;
    return result;
}

HOOK(0x00447990, S_00446340_SerializeEntries, "auto");
