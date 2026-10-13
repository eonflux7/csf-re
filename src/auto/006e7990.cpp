#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, Archive_GrowTwentyByteRecordArray, 0x006E7000, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Archive_ProcessTwentyByteRecord, 0x006E75F0, (void* self, void* edx, int a2));

static int __stdcall Archive_SerializeTwentyByteRecordArray(int* container, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    const bool writing = CSF_AT(std::uint8_t, archive, 0x14) != 0;
    const std::uint8_t savedMode = CSF_AT(std::uint8_t, archive, 0x15);

    alignas(8) std::uint8_t countBytes[0x4];
    std::uint32_t& count = *reinterpret_cast<std::uint32_t*>(countBytes);
    count = writing ? CSF_AT(std::uint32_t, container, 0x8) : 0;

    bool failed = false;
    if (CSF_AT(std::uint8_t, archive, 0x0) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x4);
        failed = !((bool(__fastcall*)(void*, void*, std::uint32_t*, int))
                      CSF_AT(void**, serializer, 0x0)[0x78 / 4])(
            serializer, nullptr, &count, archiveAddress);
    }
    CSF_AT(std::uint8_t, archive, 0x0) = failed;

    if (!failed)
    {
        if (writing || savedMode != 0)
        {
            std::uint32_t index = 0;
            std::uint32_t byteOffset = 0;
            while (index < count)
            {
                void* record = static_cast<std::uint8_t*>(
                    CSF_AT(void*, container, 0x0)) + byteOffset;
                const bool canProcess = CSF_AT(std::uint8_t, archive, 0x0) == 0;
                CSF_AT(std::uint32_t, archive, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(std::uint8_t, archive, 0x15) = savedMode;
                    CSF_AT(std::uint8_t, archive, 0x16) = 0;
                    Archive_ProcessTwentyByteRecord(record, nullptr, archiveAddress);
                }
                ++index;
                byteOffset += 20;
            }
        }
        else
        {
            const std::uint32_t capacity = CSF_AT(std::uint32_t, container, 0x4);
            std::uint32_t requestedCount = count;
            CSF_AT(std::uint32_t, container, 0x8) = 0;
            if (requestedCount > capacity)
            {
                Archive_GrowTwentyByteRecordArray(
                    container, nullptr, static_cast<int>(requestedCount - capacity));
                requestedCount = count;
            }

            std::uint32_t index = 0;
            std::uint32_t byteOffset = 0;
            if (requestedCount != 0)
            {
                do
                {
                    if (CSF_AT(std::uint32_t, container, 0x8) >=
                        CSF_AT(std::uint32_t, container, 0x4))
                    {
                        Archive_GrowTwentyByteRecordArray(
                            container, nullptr, CSF_AT(int, container, 0xC));
                    }

                    const std::uint32_t insertionIndex =
                        CSF_AT(std::uint32_t, container, 0x8);
                    void* data = CSF_AT(void*, container, 0x0);
                    CSF_AT(std::uint32_t, container, 0x8) = insertionIndex + 1;
                    void* insertedRecord =
                        static_cast<std::uint8_t*>(data) + 20 * insertionIndex;
                    CSF_AT(std::uint32_t, insertedRecord, 0x0) = 0;
                    CSF_AT(std::uint32_t, insertedRecord, 0x4) = 0;
                    CSF_AT(std::uint32_t, insertedRecord, 0x8) = 0;
                    CSF_AT(std::uint32_t, insertedRecord, 0xC) = 0;
                    CSF_AT(std::uint32_t, insertedRecord, 0x10) = 0;

                    void* record = static_cast<std::uint8_t*>(
                        CSF_AT(void*, container, 0x0)) + byteOffset;
                    const bool canProcess = CSF_AT(std::uint8_t, archive, 0x0) == 0;
                    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
                    if (canProcess)
                    {
                        CSF_AT(std::uint8_t, archive, 0x15) = 0;
                        CSF_AT(std::uint8_t, archive, 0x16) = 0;
                        Archive_ProcessTwentyByteRecord(record, nullptr, archiveAddress);
                    }

                    byteOffset += 20;
                    ++index;
                } while (index < count);
            }
        }

        if (CSF_AT(std::uint8_t, archive, 0x0) == 0)
        {
            void* serializer = CSF_AT(void*, archive, 0x4);
            if (((bool(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, serializer, 0x0)[0x7C / 4])(
                    serializer, nullptr, archiveAddress))
            {
                CSF_AT(std::uint8_t, archive, 0x0) = 0;
                return 0;
            }
        }
    }

    CSF_AT(std::uint8_t, archive, 0x0) = 1;
    return 1;
}

HOOK(0x006E7990, Archive_SerializeTwentyByteRecordArray, "auto");
