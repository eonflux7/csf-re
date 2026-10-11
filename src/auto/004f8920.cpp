#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharacterModel_GrowEntryArray, 0x004F7D10,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharacterModel_ProcessEntry, 0x004F8320,
         (void* self, void* edx, void* a2));

static unsigned int __stdcall CharacterModel_ProcessEntryArray(
    S_004f7d10* entries, int contextAddress)
{
    // The packet also identifies the growth routine's address as a vtable.
    (void)CSF_VTBL(0x004F7D10);

    auto* context = reinterpret_cast<uint8_t*>(contextAddress);
    const uint8_t updateExisting = CSF_AT(uint8_t, context, 0x15);
    const uint8_t useStoredCount = CSF_AT(uint8_t, context, 0x14);
    const uint8_t previousFailure = CSF_AT(uint8_t, context, 0x00);

    alignas(8) uint8_t countBytes[0x4] = {};
    auto& count = *reinterpret_cast<uint32_t*>(countBytes);
    if (useStoredCount)
        count = entries->field_0008;

    uint32_t result = 0;
    if (!previousFailure)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        const bool succeeded =
            reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, stream, 0)[120 / 4])(
                    stream, nullptr, &count, context);
        result = !succeeded;
    }
    CSF_AT(uint8_t, context, 0x00) = static_cast<uint8_t>(result);

    if (static_cast<uint8_t>(result))
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return result;
    }

    if (useStoredCount || updateExisting)
    {
        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < count)
        {
            auto* entry =
                reinterpret_cast<uint8_t*>(entries->field_0000) + offset;
            const bool process = CSF_AT(uint8_t, context, 0x00) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (process)
            {
                CSF_AT(uint8_t, context, 0x15) = updateExisting;
                CSF_AT(uint8_t, context, 0x16) = 0;
                CharacterModel_ProcessEntry(entry, nullptr, context);
            }
            result = index + 1;
            offset += 48;
            ++index;
        }
    }
    else
    {
        result = entries->field_0004;
        entries->field_0008 = 0;
        if (count > result)
        {
            result = reinterpret_cast<uintptr_t>(
                CharacterModel_GrowEntryArray(
                    entries, nullptr, static_cast<int>(count - result)));
        }

        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < count)
        {
            if (entries->field_0008 >= entries->field_0004)
            {
                CharacterModel_GrowEntryArray(
                    entries, nullptr, static_cast<int>(entries->field_000c));
            }

            const uint32_t entryIndex = entries->field_0008;
            auto* newEntry = reinterpret_cast<uint8_t*>(entries->field_0000)
                           + 48 * entryIndex;
            entries->field_0008 = entryIndex + 1;

            CSF_AT(int32_t, newEntry, 0x00) = -1;
            CSF_AT(int32_t, newEntry, 0x08) = -1;
            CSF_AT(int32_t, newEntry, 0x0C) = -1;
            CSF_AT(int32_t, newEntry, 0x14) = -1;
            CSF_AT(int32_t, newEntry, 0x18) = -1;
            CSF_AT(uint32_t, newEntry, 0x20) = 0;
            CSF_AT(uint32_t, newEntry, 0x24) = 0;
            CSF_AT(uint32_t, newEntry, 0x28) = 0;
            CSF_AT(uint32_t, newEntry, 0x1C) = 0;
            CSF_AT(uint32_t, newEntry, 0x2C) = 0;

            auto* entry =
                reinterpret_cast<uint8_t*>(entries->field_0000) + offset;
            const bool process = CSF_AT(uint8_t, context, 0x00) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (process)
            {
                CSF_AT(uint8_t, context, 0x15) = 0;
                CSF_AT(uint8_t, context, 0x16) = 0;
                CharacterModel_ProcessEntry(entry, nullptr, context);
            }

            result = index + 1;
            offset += 48;
            ++index;
        }
    }

    uint8_t failed = 1;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, stream, 0)[124 / 4])(stream, nullptr, context);
        failed = static_cast<uint8_t>(result) == 0;
    }
    CSF_AT(uint8_t, context, 0x00) = failed;
    return result;
}

HOOK(0x004F8920, CharacterModel_ProcessEntryArray, "auto");
