#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SerializeEntry, 0x00448920, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, GrowEntryStorage, 0x00448A80, (void* self, void* edx, int a2));

static int __stdcall S_00448a80_SerializeEntries(S_00448a80* entries, void* context)
{
    // The packet also identifies the growth function address as a vtable.
    (void)CSF_VTBL(0x00448A80);

    const bool writing = CSF_AT(uint8_t, context, 0x14) != 0;
    const uint8_t updateMode = CSF_AT(uint8_t, context, 0x15);

    alignas(8) uint8_t countBytes[0x4] = {};
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);
    if (writing)
        count = entries->field_0008;

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, void*)>(
            CSF_AT(void**, serializer, 0)[0x78 / 4])(
                serializer, nullptr, &count, context);
    }
    CSF_AT(uint8_t, context, 0) = failed;

    if (!failed)
    {
        if (writing || updateMode != 0)
        {
            uint32_t index = 0;
            uint32_t offset = 0;
            if (count != 0)
            {
                do
                {
                    void* entry = reinterpret_cast<uint8_t*>(entries->field_0000) + offset;
                    const bool ready = CSF_AT(uint8_t, context, 0) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (ready)
                    {
                        CSF_AT(uint8_t, context, 0x15) = updateMode;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        SerializeEntry(entry, nullptr, context);
                    }
                    ++index;
                    offset += 24;
                } while (index < count);
            }
        }
        else
        {
            const uint32_t capacity = entries->field_0004;
            entries->field_0008 = 0;
            if (count > capacity)
                GrowEntryStorage(entries, nullptr, static_cast<int>(count - capacity));

            uint32_t index = 0;
            uint32_t offset = 0;
            if (count != 0)
            {
                do
                {
                    if (entries->field_0008 >= entries->field_0004)
                        GrowEntryStorage(entries, nullptr, static_cast<int>(entries->field_000c));

                    const uint32_t used = entries->field_0008;
                    void* newEntry = reinterpret_cast<uint8_t*>(entries->field_0000) + 24 * used;
                    entries->field_0008 = used + 1;
                    CSF_AT(int32_t, newEntry, 0x4) = -1;
                    CSF_AT(int32_t, newEntry, 0x8) = -1;
                    CSF_AT(int32_t, newEntry, 0x10) = -1;
                    CSF_AT(int32_t, newEntry, 0x14) = -1;

                    void* entry = reinterpret_cast<uint8_t*>(entries->field_0000) + offset;
                    const bool ready = CSF_AT(uint8_t, context, 0) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (ready)
                    {
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        SerializeEntry(entry, nullptr, context);
                    }
                    ++index;
                    offset += 24;
                } while (index < count);
            }
        }

        if (CSF_AT(uint8_t, context, 0) == 0)
        {
            void* serializer = CSF_AT(void*, context, 0x4);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, serializer, 0)[0x7C / 4])(
                        serializer, nullptr, context))
            {
                CSF_AT(uint8_t, context, 0) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004496C0, S_00448a80_SerializeEntries, "auto");
