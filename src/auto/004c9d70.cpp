#include <cstdint>
#include <cstring>
#include <cfloat>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CoverAttackTasks_GrowRecordArray, 0x004ADE60,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoverAttackTasks_ProcessRecord, 0x004C3530,
         (void* self, void* edx, int a2));

static int __stdcall CoverAttackTasks_ProcessRecordArray(void* records, char* context)
{
    const char savedMode = CSF_AT(char, context, 0x15);
    const bool reuseRecords = CSF_AT(char, context, 0x14) != 0;
    const char initialState = CSF_AT(char, context, 0);
    std::uint32_t recordCount = 0;

    if (reuseRecords)
        recordCount = CSF_AT(std::uint32_t, records, 0x08);

    bool failed = false;
    if (!initialState)
    {
        void* owner = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, std::uint32_t*, char*))
                       CSF_AT(void**, owner, 0)[0x78 / 4])(
                           owner, nullptr, &recordCount, context);
    }
    CSF_AT(char, context, 0) = failed;

    if (failed)
    {
        CSF_AT(char, context, 0) = 1;
        return 1;
    }

    if (reuseRecords || savedMode)
    {
        for (std::uint32_t index = 0; index < recordCount; ++index)
        {
            auto* record = CSF_AT(std::uint8_t*, records, 0)
                         + index * 0x1C;
            const bool process = CSF_AT(char, context, 0) == 0;
            CSF_AT(std::uint32_t, context, 0x1C) = 0;
            if (process)
            {
                CSF_AT(char, context, 0x15) = savedMode;
                CSF_AT(char, context, 0x16) = 0;
                CoverAttackTasks_ProcessRecord(
                    record, nullptr,
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(context)));
            }
        }
    }
    else
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, records, 0x04);
        CSF_AT(std::uint32_t, records, 0x08) = 0;
        if (recordCount > capacity)
        {
            CoverAttackTasks_GrowRecordArray(
                records, nullptr, static_cast<int>(recordCount - capacity));
        }

        if (recordCount)
        {
            std::uint8_t defaultRecord[0x1C] = {};
            CSF_AT(float, defaultRecord, 0x18) = FLT_MAX;

            for (std::uint32_t index = 0; index < recordCount; ++index)
            {
                if (CSF_AT(std::uint32_t, records, 0x08)
                    >= CSF_AT(std::uint32_t, records, 0x04))
                {
                    CoverAttackTasks_GrowRecordArray(
                        records, nullptr,
                        static_cast<int>(CSF_AT(std::uint32_t, records, 0x0C)));
                }

                const std::uint32_t size = CSF_AT(std::uint32_t, records, 0x08);
                auto* destination = CSF_AT(std::uint8_t*, records, 0)
                                  + size * 0x1C;
                CSF_AT(std::uint32_t, records, 0x08) = size + 1;
                std::memcpy(destination, defaultRecord, 0x1C);

                auto* record = CSF_AT(std::uint8_t*, records, 0)
                             + index * 0x1C;
                const bool process = CSF_AT(char, context, 0) == 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (process)
                {
                    CSF_AT(char, context, 0x15) = 0;
                    CSF_AT(char, context, 0x16) = 0;
                    CoverAttackTasks_ProcessRecord(
                        record, nullptr,
                        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)));
                }
            }
        }
    }

    if (!CSF_AT(char, context, 0))
    {
        void* owner = CSF_AT(void*, context, 0x04);
        if (((bool(__fastcall*)(void*, void*, char*))
                 CSF_AT(void**, owner, 0)[0x7C / 4])(owner, nullptr, context))
        {
            CSF_AT(char, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(char, context, 0) = 1;
    return 1;
}

HOOK(0x004C9D70, CoverAttackTasks_ProcessRecordArray, "auto");
