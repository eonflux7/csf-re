#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using RecordDestructorResult = int (__stdcall **)(char);

CSF_FUNC(int, __fastcall, SceneLoader_InitializeRecord, 0x004FF360, (void* self, void* edx));
CSF_FUNC(RecordDestructorResult, __fastcall, SceneLoader_DestroyRecord, 0x004FF490, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ClearRecords, 0x00502760, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_ReserveRecords, 0x005027B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneLoader_SerializeRecord, 0x005033B0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_AppendRecord, 0x00503A10, (void* self, void* edx, int a3));

static int __stdcall SceneLoader_SerializeRecords(S_00503500* records, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t writing = CSF_AT(uint8_t, context, 0x14);
    const uint8_t updating = CSF_AT(uint8_t, context, 0x15);
    unsigned int count = 0;

    if (writing)
    {
        uint32_t loopCount = records->field_0004;
        if (loopCount)
            loopCount = static_cast<int32_t>(records->field_0008 - loopCount) / 88;
        count = loopCount;

        bool failed = false;
        if (CSF_AT(uint8_t, context, 0x00) == 0)
        {
            void* stream = CSF_AT(void*, context, 0x04);
            failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, unsigned int*, int)>(
                CSF_AT(void**, stream, 0)[120 / 4])(
                    stream, nullptr, &count, contextAddress);
            loopCount = count;
        }
        CSF_AT(uint8_t, context, 0x00) = failed;

        if (!failed)
        {
            uint32_t index = 0;
            uint32_t offset = 0;
            while (index < loopCount)
            {
                void* record = reinterpret_cast<void*>(records->field_0004 + offset);
                const bool canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(uint8_t, context, 0x15) = updating;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    SceneLoader_SerializeRecord(record, nullptr, contextAddress);
                    loopCount = count;
                }
                ++index;
                offset += 88;
            }

            if (CSF_AT(uint8_t, context, 0x00) == 0)
            {
                void* stream = CSF_AT(void*, context, 0x04);
                if (reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, stream, 0)[124 / 4])(
                            stream, nullptr, contextAddress))
                {
                    CSF_AT(uint8_t, context, 0x00) = 0;
                    return 0;
                }
            }
        }
    }
    else
    {
        bool failed = false;
        if (CSF_AT(uint8_t, context, 0x00) == 0)
        {
            void* stream = CSF_AT(void*, context, 0x04);
            failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, unsigned int*, int)>(
                CSF_AT(void**, stream, 0)[120 / 4])(
                    stream, nullptr, &count, contextAddress);
        }
        CSF_AT(uint8_t, context, 0x00) = failed;

        if (!failed)
        {
            if (updating)
            {
                uint32_t index = 0;
                uint32_t offset = 0;
                while (index < count)
                {
                    void* record = reinterpret_cast<void*>(records->field_0004 + offset);
                    const bool canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (canSerialize)
                    {
                        CSF_AT(uint8_t, context, 0x15) = updating;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        SceneLoader_SerializeRecord(record, nullptr, contextAddress);
                    }
                    ++index;
                    offset += 88;
                }
            }
            else
            {
                SceneLoader_ClearRecords(records, nullptr);
                SceneLoader_ReserveRecords(records, nullptr, count);

                uint32_t temporaryRecord[22];
                uint32_t index = 0;
                uint32_t offset = 0;
                while (index < count)
                {
                    SceneLoader_InitializeRecord(temporaryRecord, nullptr);
                    SceneLoader_AppendRecord(
                        records, nullptr, reinterpret_cast<int>(temporaryRecord));

                    void* record = reinterpret_cast<void*>(records->field_0004 + offset);
                    const bool canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (canSerialize)
                    {
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        SceneLoader_SerializeRecord(record, nullptr, contextAddress);
                    }

                    SceneLoader_DestroyRecord(temporaryRecord, nullptr);
                    offset += 88;
                    ++index;
                }
            }

            if (CSF_AT(uint8_t, context, 0x00) == 0)
            {
                void* stream = CSF_AT(void*, context, 0x04);
                if (reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, stream, 0)[124 / 4])(
                            stream, nullptr, contextAddress))
                {
                    CSF_AT(uint8_t, context, 0x00) = 0;
                    return 0;
                }
            }
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00505210, SceneLoader_SerializeRecords, "auto");
