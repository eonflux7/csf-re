#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SerializeEntry, 0x00448920,
         (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, ClearEntries, 0x004538E0,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, ReserveEntries, 0x00454AF0,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(void, __fastcall, AppendEntry, 0x00455470,
         (void* self, void* edx, uint32_t* a3));

static int __stdcall Serialization_ProcessEntryCollection(
    S_00454c80* entries, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const bool writing = CSF_AT(uint8_t, context, 0x14) != 0;
    const uint8_t updateExisting = CSF_AT(uint8_t, context, 0x15);
    unsigned int count = 0;

    if (writing)
    {
        const uint32_t begin = entries->field_0004;
        if (begin)
            count = static_cast<int32_t>(entries->field_0008 - begin) / 24;
    }

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, unsigned int*, void*)>(
                CSF_AT(void**, serializer, 0)[0x78 / 4])(
                    serializer, nullptr, &count, context);
    }
    CSF_AT(uint8_t, context, 0x00) = failed;

    if (!failed)
    {
        if (writing)
        {
            unsigned int limit = count;
            unsigned int index = 0;
            uint32_t offset = 0;
            if (limit)
            {
                do
                {
                    void* entry = reinterpret_cast<void*>(
                        entries->field_0004 + offset);
                    const bool canProcess =
                        CSF_AT(uint8_t, context, 0x00) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (canProcess)
                    {
                        CSF_AT(uint8_t, context, 0x15) = updateExisting;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        SerializeEntry(entry, nullptr, context);
                        limit = count;
                    }
                    ++index;
                    offset += 24;
                } while (index < limit);
            }
        }
        else if (updateExisting)
        {
            unsigned int index = 0;
            uint32_t offset = 0;
            if (count)
            {
                do
                {
                    void* entry = reinterpret_cast<void*>(
                        entries->field_0004 + offset);
                    const bool canProcess =
                        CSF_AT(uint8_t, context, 0x00) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (canProcess)
                    {
                        CSF_AT(uint8_t, context, 0x15) = updateExisting;
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
            ClearEntries(entries, nullptr);
            ReserveEntries(entries, nullptr, count);

            unsigned int index = 0;
            uint32_t offset = 0;
            uint32_t temporaryEntry[6];
            if (count)
            {
                do
                {
                    temporaryEntry[0] =
                        reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
                    temporaryEntry[1] = static_cast<uint32_t>(-1);
                    temporaryEntry[2] = static_cast<uint32_t>(-1);
                    temporaryEntry[3] =
                        reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
                    temporaryEntry[4] = static_cast<uint32_t>(-1);
                    temporaryEntry[5] = static_cast<uint32_t>(-1);
                    AppendEntry(entries, nullptr, temporaryEntry);

                    void* entry = reinterpret_cast<void*>(
                        entries->field_0004 + offset);
                    const bool canProcess =
                        CSF_AT(uint8_t, context, 0x00) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (canProcess)
                    {
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        SerializeEntry(entry, nullptr, context);
                    }

                    temporaryEntry[3] =
                        reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                    temporaryEntry[0] =
                        reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                    ++index;
                    offset += 24;
                } while (index < count);
            }
        }

        if (CSF_AT(uint8_t, context, 0x00) == 0)
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, serializer, 0)[0x7C / 4])(
                        serializer, nullptr, context))
            {
                CSF_AT(uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00455A40, Serialization_ProcessEntryCollection, "auto");
