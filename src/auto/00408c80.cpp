#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, MatrixArray_Grow, 0x004085C0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MatrixRecord_Process, 0x00408AD0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10,
         (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

static int __stdcall MatrixArray_ProcessRecords(void* records, int contextAddress)
{
    auto* context = reinterpret_cast<void*>(contextAddress);
    const std::uint8_t savedMode = CSF_AT(std::uint8_t, context, 0x15);
    const bool useExistingCount = CSF_AT(std::uint8_t, context, 0x14) != 0;
    const std::uint8_t previousStatus = CSF_AT(std::uint8_t, context, 0x00);

    std::uint32_t count = 0;
    if (useExistingCount)
        count = CSF_AT(std::uint32_t, records, 0x08);

    bool failed = false;
    if (!previousStatus)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, std::uint32_t*, int)>(
                CSF_AT(void**, processor, 0x00)[0x78 / 4])(
                    processor, nullptr, &count, contextAddress);
    }
    CSF_AT(std::uint8_t, context, 0x00) = failed;

    if (!failed)
    {
        if (useExistingCount || savedMode)
        {
            std::uint32_t offset = 0;
            for (std::uint32_t index = 0; index < count; ++index, offset += 28)
            {
                auto* record = CSF_AT(std::uint8_t*, records, 0x00) + offset;
                const bool process = CSF_AT(std::uint8_t, context, 0x00) == 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (process)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = savedMode;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    MatrixRecord_Process(record, nullptr, contextAddress);
                }
            }
        }
        else
        {
            const std::uint32_t capacity = CSF_AT(std::uint32_t, records, 0x04);
            CSF_AT(std::uint32_t, records, 0x08) = 0;
            if (count > capacity)
                MatrixArray_Grow(records, nullptr, static_cast<int>(count - capacity));

            std::uint32_t offset = 0;
            for (std::uint32_t index = 0; index < count; ++index, offset += 28)
            {
                std::uint32_t* matrix = RwMatrixCreate();
                if (CSF_AT(std::uint32_t, records, 0x08) >=
                    CSF_AT(std::uint32_t, records, 0x04))
                {
                    MatrixArray_Grow(records, nullptr,
                                     CSF_AT(int, records, 0x0C));
                }

                const std::uint32_t used = CSF_AT(std::uint32_t, records, 0x08);
                auto* record = CSF_AT(std::uint8_t*, records, 0x00) + 28 * used;
                CSF_AT(std::uint32_t, records, 0x08) = used + 1;
                CSF_AT(std::uint32_t, record, 0x00) = 0;
                CSF_AT(std::uint32_t, record, 0x04) = 0;
                void* destination = CSF_AT(void*, record, 0x08);
                std::memcpy(destination, matrix, 0x40);
                CSF_AT(std::uint32_t, record, 0x10) = 0;
                CSF_AT(std::uint32_t, record, 0x14) = 0;
                CSF_AT(std::uint32_t, record, 0x18) = 0;

                auto* currentRecord = CSF_AT(std::uint8_t*, records, 0x00) + offset;
                const bool process = CSF_AT(std::uint8_t, context, 0x00) == 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (process)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = 0;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    MatrixRecord_Process(currentRecord, nullptr, contextAddress);
                }
                RwMatrixDestroy(matrix);
            }
        }

        if (!CSF_AT(std::uint8_t, context, 0x00))
        {
            void* processor = CSF_AT(void*, context, 0x04);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, processor, 0x00)[0x7C / 4])(
                        processor, nullptr, contextAddress))
            {
                CSF_AT(std::uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00408C80, MatrixArray_ProcessRecords, "auto");
