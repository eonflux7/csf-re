#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, GrowEntryArray, 0x00446450,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SerializeEntry, 0x00447480,
         (void* self, void* edx, void* a2));

[[maybe_unused]] static const auto kEntryArrayVtable = CSF_VTBL(0x00446450);

static uint32_t __stdcall S_00446450_SerializeEntries(
    S_00446450* entries, void* archive)
{
    const uint8_t savedMode = CSF_AT(uint8_t, archive, 0x15);
    const bool writing = CSF_AT(uint8_t, archive, 0x14) != 0;
    const uint8_t previousError = CSF_AT(uint8_t, archive, 0x00);

    uint32_t count = writing ? entries->field_0008 : 0;
    uint32_t result = 0;

    if (!previousError)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        const bool success =
            ((bool(__fastcall*)(void*, void*, uint32_t*, void*))
                 CSF_AT(void**, stream, 0)[0x78 / 4])(
                stream, nullptr, &count, archive);
        result = !success;
    }

    CSF_AT(uint8_t, archive, 0x00) = static_cast<uint8_t>(result);
    if (result)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return result;
    }

    if (writing || savedMode)
    {
        uint32_t offset = 0;
        for (uint32_t index = 0; index < count;)
        {
            void* entry = reinterpret_cast<void*>(entries->field_0000 + offset);
            const bool canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
            CSF_AT(uint32_t, archive, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, archive, 0x15) = savedMode;
                CSF_AT(uint8_t, archive, 0x16) = 0;
                SerializeEntry(entry, nullptr, archive);
            }
            result = ++index;
            offset += 40;
        }
    }
    else
    {
        result = entries->field_0004;
        entries->field_0008 = 0;
        if (count > result)
        {
            result = reinterpret_cast<uint32_t>(
                GrowEntryArray(entries, nullptr, static_cast<int>(count - result)));
        }

        uint32_t offset = 0;
        for (uint32_t index = 0; index < count;)
        {
            if (entries->field_0008 >= entries->field_0004)
            {
                GrowEntryArray(entries, nullptr,
                               static_cast<int>(entries->field_000c));
            }

            const uint32_t slot = entries->field_0008;
            void* entry = reinterpret_cast<void*>(entries->field_0000 + 40 * slot);
            entries->field_0008 = slot + 1;
            CSF_AT(int32_t, entry, 0x00) = -1;
            CSF_AT(int32_t, entry, 0x04) = -1;
            CSF_AT(int32_t, entry, 0x0C) = -1;
            CSF_AT(int32_t, entry, 0x10) = -1;
            CSF_AT(uint32_t, entry, 0x14) = 0;
            CSF_AT(uint32_t, entry, 0x18) = 0;
            CSF_AT(uint32_t, entry, 0x1C) = 0;
            CSF_AT(float, entry, 0x20) = 1.0f;
            CSF_AT(uint32_t, entry, 0x24) = 0;

            entry = reinterpret_cast<void*>(entries->field_0000 + offset);
            const bool canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
            CSF_AT(uint32_t, archive, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, archive, 0x15) = 0;
                CSF_AT(uint8_t, archive, 0x16) = 0;
                SerializeEntry(entry, nullptr, archive);
            }
            result = ++index;
            offset += 40;
        }
    }

    uint8_t error = 1;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        result = ((uint32_t(__fastcall*)(void*, void*, void*))
                      CSF_AT(void**, stream, 0)[0x7C / 4])(
            stream, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
            error = 0;
    }

    CSF_AT(uint8_t, archive, 0x00) = error;
    return result;
}

HOOK(0x00447B80, S_00446450_SerializeEntries, "auto");
