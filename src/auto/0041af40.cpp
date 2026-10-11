#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, Vehicle_GrowRecordArray, 0x00418950,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_SerializeRecord, 0x0041A950,
         (void* self, void* edx, int a2));

static int __stdcall Vehicle_SerializeRecordArray(
    std::uint32_t* records, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const std::uint8_t savedMode = CSF_AT(std::uint8_t, context, 0x15);
    const std::uint8_t initialError = CSF_AT(std::uint8_t, context, 0x00);
    const bool useExistingCount = CSF_AT(std::uint8_t, context, 0x14) != 0;

    std::uint32_t count = 0;
    if (useExistingCount)
        count = CSF_AT(std::uint32_t, records, 0x08);

    bool error = false;
    if (!initialError)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        error = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, std::uint32_t*, int)>(
                CSF_AT(void**, handler, 0x00)[120 / 4])(
                    handler, nullptr, &count, contextAddress);
    }
    CSF_AT(std::uint8_t, context, 0x00) = error;

    if (!error)
    {
        if (useExistingCount || savedMode)
        {
            std::uint32_t index = 0;
            std::uint32_t byteOffset = 0;
            if (count)
            {
                do
                {
                    void* record =
                        CSF_AT(std::uint8_t*, records, 0x00) + byteOffset;
                    const bool process =
                        CSF_AT(std::uint8_t, context, 0x00) == 0;
                    CSF_AT(std::uint32_t, context, 0x1C) = 0;
                    if (process)
                    {
                        CSF_AT(std::uint8_t, context, 0x15) = savedMode;
                        CSF_AT(std::uint8_t, context, 0x16) = 0;
                        Vehicle_SerializeRecord(record, nullptr, contextAddress);
                    }
                    ++index;
                    byteOffset += 32;
                } while (index < count);
            }
        }
        else
        {
            const std::uint32_t capacity =
                CSF_AT(std::uint32_t, records, 0x04);
            std::uint32_t requestedCount = count;
            CSF_AT(std::uint32_t, records, 0x08) = 0;
            if (requestedCount > capacity)
            {
                Vehicle_GrowRecordArray(
                    records, nullptr, static_cast<int>(requestedCount - capacity));
                requestedCount = count;
            }

            std::uint32_t index = 0;
            std::uint32_t byteOffset = 0;
            if (requestedCount)
            {
                do
                {
                    const std::uint32_t used =
                        CSF_AT(std::uint32_t, records, 0x08);
                    std::uint32_t emptyRecord[8];
                    std::memset(emptyRecord, 0, sizeof(emptyRecord));
                    if (used >= CSF_AT(std::uint32_t, records, 0x04))
                    {
                        Vehicle_GrowRecordArray(
                            records, nullptr,
                            static_cast<int>(
                                CSF_AT(std::uint32_t, records, 0x0C)));
                    }

                    const std::uint32_t insertionIndex =
                        CSF_AT(std::uint32_t, records, 0x08);
                    void* destination =
                        CSF_AT(std::uint8_t*, records, 0x00) + 32 * insertionIndex;
                    CSF_AT(std::uint32_t, records, 0x08) = insertionIndex + 1;
                    std::memcpy(destination, emptyRecord, 0x20);

                    void* record =
                        CSF_AT(std::uint8_t*, records, 0x00) + byteOffset;
                    const bool process =
                        CSF_AT(std::uint8_t, context, 0x00) == 0;
                    CSF_AT(std::uint32_t, context, 0x1C) = 0;
                    if (process)
                    {
                        CSF_AT(std::uint8_t, context, 0x15) = 0;
                        CSF_AT(std::uint8_t, context, 0x16) = 0;
                        Vehicle_SerializeRecord(record, nullptr, contextAddress);
                    }

                    ++index;
                    byteOffset += 32;
                } while (index < count);
            }
        }

        if (!CSF_AT(std::uint8_t, context, 0x00))
        {
            void* handler = CSF_AT(void*, context, 0x04);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, handler, 0x00)[124 / 4])(
                        handler, nullptr, contextAddress))
            {
                CSF_AT(std::uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0041AF40, Vehicle_SerializeRecordArray, "auto");
