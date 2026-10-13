#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, RecordArray_Grow, 0x006E6B80, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Record_Transfer, 0x006E76C0, (void* self, void* edx, int a2));

static int __stdcall RecordArray_Transfer(void* records, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const std::uint8_t existingMode = CSF_AT(std::uint8_t, context, 0x14);
    const std::uint8_t savedFlag = CSF_AT(std::uint8_t, context, 0x15);

    alignas(8) std::uint8_t countBytes[0x4] = {};
    auto& count = *reinterpret_cast<std::uint32_t*>(countBytes);

    const bool canBegin = CSF_AT(std::uint8_t, context, 0x0) == 0;
    if (existingMode)
        count = CSF_AT(std::uint32_t, records, 0x8);

    bool failed = false;
    if (canBegin)
    {
        void* stream = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, std::uint32_t*, int)>(
            CSF_AT(void**, stream, 0x0)[120 / 4])(
                stream, nullptr, &count, contextAddress);
    }
    CSF_AT(std::uint8_t, context, 0x0) = failed;

    if (failed)
    {
        CSF_AT(std::uint8_t, context, 0x0) = 1;
        return 1;
    }

    if (existingMode || savedFlag)
    {
        std::uint32_t index = 0;
        std::uint32_t byteOffset = 0;
        if (count)
        {
            do
            {
                void* record = CSF_AT(std::uint8_t*, records, 0x0) + byteOffset;
                const bool canTransfer = CSF_AT(std::uint8_t, context, 0x0) == 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (canTransfer)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = savedFlag;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    Record_Transfer(record, nullptr, contextAddress);
                }
                ++index;
                byteOffset += 16;
            }
            while (index < count);
        }
    }
    else
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, records, 0x4);
        std::uint32_t requestedCount = count;
        CSF_AT(std::uint32_t, records, 0x8) = 0;
        if (requestedCount > capacity)
        {
            RecordArray_Grow(records, nullptr, static_cast<int>(requestedCount - capacity));
            requestedCount = count;
        }

        std::uint32_t byteOffset = 0;
        std::uint32_t index = 0;
        if (requestedCount)
        {
            do
            {
                if (CSF_AT(std::uint32_t, records, 0x8) >=
                    CSF_AT(std::uint32_t, records, 0x4))
                {
                    RecordArray_Grow(records, nullptr, CSF_AT(int, records, 0xC));
                }

                void* appendedRecord = CSF_AT(std::uint8_t*, records, 0x0) +
                    16 * CSF_AT(std::uint32_t, records, 0x8);
                ++CSF_AT(std::uint32_t, records, 0x8);
                CSF_AT(std::uint32_t, appendedRecord, 0x0) = 0;
                CSF_AT(int, appendedRecord, 0x4) = -1;
                CSF_AT(int, appendedRecord, 0x8) = -1;
                CSF_AT(int, appendedRecord, 0xC) = -1;

                void* record = CSF_AT(std::uint8_t*, records, 0x0) + byteOffset;
                const bool canTransfer = CSF_AT(std::uint8_t, context, 0x0) == 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (canTransfer)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = 0;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    Record_Transfer(record, nullptr, contextAddress);
                }
                byteOffset += 16;
                ++index;
            }
            while (index < count);
        }
    }

    if (!CSF_AT(std::uint8_t, context, 0x0))
    {
        void* stream = CSF_AT(void*, context, 0x4);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stream, 0x0)[124 / 4])(
                    stream, nullptr, contextAddress))
        {
            CSF_AT(std::uint8_t, context, 0x0) = 0;
            return 0;
        }
    }

    CSF_AT(std::uint8_t, context, 0x0) = 1;
    return 1;
}

HOOK(0x006E77B0, RecordArray_Transfer, "auto");
