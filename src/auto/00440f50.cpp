#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int*, __fastcall, Campaign_AdvanceRecordIterator, 0x0043E190, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Campaign_DestroyRecordNodes, 0x0043F310, (void* self, void* edx, void** Block));
CSF_FUNC(void*, __stdcall, SceneLoader_SerializeRecordBase, 0x0043FAF0, (void* a1, int a2));
CSF_FUNC(int, __fastcall, Campaign_InsertRecord, 0x00440C80, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __stdcall Campaign_SerializeRecordList(
    std::uint32_t* records, std::uint32_t archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    std::uint32_t countOrNode = archiveAddress;
    int result;
    std::uint8_t failed = 0;

    if (CSF_AT(std::uint8_t, archive, 0x14))
    {
        std::uint32_t count = CSF_AT(std::uint32_t, records, 0x08);
        result = 0;
        if (!CSF_AT(std::uint8_t, archive, 0))
        {
            void* stream = CSF_AT(void*, archive, 0x04);
            result = !reinterpret_cast<bool(__fastcall*)(
                void*, void*, std::uint32_t*, void*)>(
                    CSF_AT(void**, stream, 0)[0x78 / 4])(
                        stream, nullptr, &count, archive);
        }
        CSF_AT(std::uint8_t, archive, 0) = static_cast<std::uint8_t>(result);
        if (result)
        {
            CSF_AT(std::uint8_t, archive, 0) = 1;
            return result;
        }

        void* sentinel = CSF_AT(void*, records, 0x04);
        void* node = CSF_AT(void*, sentinel, 0);
        countOrNode = reinterpret_cast<std::uint32_t>(node);
        result = reinterpret_cast<int>(sentinel);

        if (node != sentinel)
        {
            do
            {
                bool ready = CSF_AT(std::uint8_t, archive, 0) == 0;
                CSF_AT(std::uint32_t, archive, 0x1C) = 0;
                if (ready)
                {
                    CSF_AT(std::uint8_t, archive, 0x15) = 0;
                    CSF_AT(std::uint8_t, archive, 0x16) = 0;
                    SceneLoader_SerializeRecordBase(
                        static_cast<std::uint8_t*>(node) + 0x0C,
                        static_cast<int>(archiveAddress));
                }

                ready = CSF_AT(std::uint8_t, archive, 0) == 0;
                CSF_AT(std::uint32_t, archive, 0x1C) = 0;
                if (ready)
                {
                    void* stream = CSF_AT(void*, archive, 0x04);
                    CSF_AT(std::uint8_t, archive, 0x15) = 0;
                    CSF_AT(std::uint8_t, archive, 0x16) = 0;
                    CSF_AT(std::uint8_t, archive, 0) =
                        reinterpret_cast<int(__fastcall*)(
                            void*, void*, void*, void*)>(
                                CSF_AT(void**, stream, 0)[0x54 / 4])(
                                    stream, nullptr,
                                    static_cast<std::uint8_t*>(node) + 0x28,
                                    archive) == -1;
                }

                Campaign_AdvanceRecordIterator(&countOrNode, nullptr);
                result = reinterpret_cast<int>(records);
                node = reinterpret_cast<void*>(countOrNode);
            }
            while (node != CSF_AT(void*, records, 0x04));
        }
    }
    else
    {
        result = 0;
        if (!CSF_AT(std::uint8_t, archive, 0))
        {
            void* stream = CSF_AT(void*, archive, 0x04);
            result = !reinterpret_cast<bool(__fastcall*)(
                void*, void*, std::uint32_t*, void*)>(
                    CSF_AT(void**, stream, 0)[0x78 / 4])(
                        stream, nullptr, &countOrNode, archive);
        }
        CSF_AT(std::uint8_t, archive, 0) = static_cast<std::uint8_t>(result);
        if (result)
        {
            CSF_AT(std::uint8_t, archive, 0) = 1;
            return result;
        }

        Campaign_DestroyRecordNodes(
            records, nullptr,
            CSF_AT(void**, CSF_AT(void*, records, 0x04), 0x04));

        void* sentinel = CSF_AT(void*, records, 0x04);
        CSF_AT(void*, sentinel, 0x04) = sentinel;
        sentinel = CSF_AT(void*, records, 0x04);
        CSF_AT(std::uint32_t, records, 0x08) = 0;
        CSF_AT(void*, sentinel, 0) = sentinel;
        sentinel = CSF_AT(void*, records, 0x04);
        CSF_AT(void*, sentinel, 0x08) = sentinel;
        result = reinterpret_cast<int>(sentinel);

        alignas(8) std::uint8_t copyBytes[0x20];
        alignas(8) std::uint8_t recordBytes[0x20];
        C_0082d410& copy = *reinterpret_cast<C_0082d410*>(copyBytes);
        C_0082d410& record = *reinterpret_cast<C_0082d410*>(recordBytes);
        std::uint32_t insertedNode = 0;
        std::uint32_t index = 0;

        while (index < countOrNode)
        {
            bool ready = CSF_AT(std::uint8_t, archive, 0) == 0;
            CSF_AT(std::uint32_t, &record, 0x18) = 15;
            CSF_AT(std::uint32_t, &record, 0x14) = 0;
            reinterpret_cast<std::uint8_t*>(&record.field_0004)[0] = 0;
            CSF_AT(std::uint32_t, &record, 0x1C) = 0;
            CSF_AT(std::uint32_t, archive, 0x1C) = 0;
            if (ready)
            {
                CSF_AT(std::uint8_t, archive, 0x15) = 0;
                CSF_AT(std::uint8_t, archive, 0x16) = 0;
                SceneLoader_SerializeRecordBase(
                    &record, static_cast<int>(archiveAddress));
            }

            CSF_AT(std::uint32_t, &copy, 0x18) = 15;
            CSF_AT(std::uint32_t, &copy, 0x14) = 0;
            reinterpret_cast<std::uint8_t*>(&copy.field_0004)[0] = 0;
            AssignStringSubstring(&copy, nullptr, &record, 0, -1);
            CSF_AT(std::uint32_t, &copy, 0x1C) =
                CSF_AT(std::uint32_t, &record, 0x1C);
            Campaign_InsertRecord(
                records, nullptr, reinterpret_cast<int>(&insertedNode),
                reinterpret_cast<void**>(&copy));

            if (CSF_AT(std::uint32_t, &copy, 0x18) >= 0x10 &&
                copy.field_0004)
            {
                Mem_FreeDebug(reinterpret_cast<void*>(copy.field_0004));
            }

            ready = CSF_AT(std::uint8_t, archive, 0) == 0;
            CSF_AT(std::uint32_t, &copy, 0x18) = 15;
            CSF_AT(std::uint32_t, &copy, 0x14) = 0;
            reinterpret_cast<std::uint8_t*>(&copy.field_0004)[0] = 0;
            CSF_AT(std::uint32_t, archive, 0x1C) = 0;
            if (ready)
            {
                void* data = reinterpret_cast<void*>(insertedNode + 0x28);
                void* stream = CSF_AT(void*, archive, 0x04);
                CSF_AT(std::uint8_t, archive, 0x15) = 0;
                CSF_AT(std::uint8_t, archive, 0x16) = 0;
                CSF_AT(std::uint8_t, archive, 0) =
                    reinterpret_cast<int(__fastcall*)(
                        void*, void*, void*, void*)>(
                            CSF_AT(void**, stream, 0)[0x54 / 4])(
                                stream, nullptr, data, archive) == -1;
            }

            if (CSF_AT(std::uint32_t, &record, 0x18) >= 0x10 &&
                record.field_0004)
            {
                Mem_FreeDebug(reinterpret_cast<void*>(record.field_0004));
            }

            ++index;
            result = static_cast<int>(index);
            CSF_AT(std::uint32_t, &record, 0x18) = 15;
            CSF_AT(std::uint32_t, &record, 0x14) = 0;
            reinterpret_cast<std::uint8_t*>(&record.field_0004)[0] = 0;
        }
    }

    if (CSF_AT(std::uint8_t, archive, 0))
    {
        failed = 1;
    }
    else
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, archive);
        if (!static_cast<std::uint8_t>(result))
            failed = 1;
    }

    CSF_AT(std::uint8_t, archive, 0) = failed;
    return result;
}

HOOK(0x00440F50, Campaign_SerializeRecordList, "auto");

