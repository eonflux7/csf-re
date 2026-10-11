#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Serialization_ProcessRecord, 0x0044ED60,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Serialization_GrowRecordArray, 0x0048EB60,
         (void* self, void* edx, int a2));

static int __stdcall Serialization_ProcessRecordArray(void* records, void* context)
{
    const std::uint8_t writing = CSF_AT(std::uint8_t, context, 0x14);
    const std::uint8_t updating = CSF_AT(std::uint8_t, context, 0x15);
    std::uint32_t count = 0;

    if (writing)
        count = CSF_AT(std::uint32_t, records, 0x08);

    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, std::uint32_t*, void*))
                       CSF_AT(void**, stream, 0x00)[0x78 / 4])(
            stream, nullptr, &count, context);
    }
    CSF_AT(std::uint8_t, context, 0x00) = failed;

    if (!failed)
    {
        if (writing || updating)
        {
            for (std::uint32_t index = 0; index < count; ++index)
            {
                auto* entry = reinterpret_cast<std::uint8_t*>(
                    CSF_AT(void*, records, 0x00)) + 12 * index;
                const bool canProcess =
                    CSF_AT(std::uint8_t, context, 0x00) == 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = updating;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    Serialization_ProcessRecord(
                        entry, nullptr, reinterpret_cast<int>(context));
                }
            }
        }
        else
        {
            const std::uint32_t capacity =
                CSF_AT(std::uint32_t, records, 0x04);
            CSF_AT(std::uint32_t, records, 0x08) = 0;
            if (count > capacity)
            {
                Serialization_GrowRecordArray(
                    records, nullptr, static_cast<int>(count - capacity));
            }

            for (std::uint32_t index = 0; index < count; ++index)
            {
                if (CSF_AT(std::uint32_t, records, 0x08) >=
                    CSF_AT(std::uint32_t, records, 0x04))
                {
                    Serialization_GrowRecordArray(
                        records, nullptr,
                        CSF_AT(int, records, 0x0C));
                }

                const std::uint32_t size =
                    CSF_AT(std::uint32_t, records, 0x08);
                auto* entry = reinterpret_cast<std::uint8_t*>(
                    CSF_AT(void*, records, 0x00)) + 12 * size;
                CSF_AT(std::uint32_t, records, 0x08) = size + 1;
                CSF_AT(int, entry, 0x00) = -1;
                CSF_AT(std::uint32_t, entry, 0x04) = 0;
                CSF_AT(std::uint32_t, entry, 0x08) = 0;

                auto* processEntry = reinterpret_cast<std::uint8_t*>(
                    CSF_AT(void*, records, 0x00)) + 12 * index;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (CSF_AT(std::uint8_t, context, 0x00) == 0)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = 0;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    Serialization_ProcessRecord(
                        processEntry, nullptr, reinterpret_cast<int>(context));
                }
            }
        }

        if (CSF_AT(std::uint8_t, context, 0x00) == 0)
        {
            void* stream = CSF_AT(void*, context, 0x04);
            if (((bool(__fastcall*)(void*, void*, void*))
                     CSF_AT(void**, stream, 0x00)[0x7C / 4])(
                    stream, nullptr, context))
            {
                CSF_AT(std::uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004916D0, Serialization_ProcessRecordArray, "auto");
