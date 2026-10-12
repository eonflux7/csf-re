#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_AppendTangent, 0x0044BA20, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(unsigned int, __fastcall, TypeRegistry_ReserveVector3Storage, 0x0054D2A0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790, (S_0047ea40* self, void*));

static int __stdcall TypeRegistry_SerializeVector3Array(
    S_0047ea40* values, void* context)
{
    const uint8_t reuseStorage = CSF_AT(uint8_t, context, 0x15);
    alignas(8) uint8_t countBytes[0x4] = {};
    auto& count = *reinterpret_cast<unsigned int*>(countBytes);
    int result = 0;

    if (CSF_AT(uint8_t, context, 0x14))
    {
        const uint32_t begin = values->field_0004;
        unsigned int loopCount = begin
            ? static_cast<int32_t>(values->field_0008 - begin) / 12
            : 0;
        count = loopCount;

        if (!CSF_AT(uint8_t, context, 0))
        {
            void* serializer = CSF_AT(void*, context, 0x4);
            const bool succeeded =
                reinterpret_cast<bool(__fastcall*)(void*, void*, unsigned int*, void*)>(
                    CSF_AT(void**, serializer, 0)[0x78 / 4])(
                        serializer, nullptr, &count, context);
            loopCount = count;
            result = !succeeded;
        }
        CSF_AT(uint8_t, context, 0) = static_cast<uint8_t>(result);
        if (result)
        {
            CSF_AT(uint8_t, context, 0) = 1;
            return result;
        }

        unsigned int index = 0;
        uint32_t byteOffset = 0;
        while (index < loopCount)
        {
            void* element = reinterpret_cast<void*>(values->field_0004 + byteOffset);
            const bool failed = CSF_AT(uint8_t, context, 0) != 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!failed)
            {
                CSF_AT(uint8_t, context, 0x15) = reuseStorage;
                void* serializer = CSF_AT(void*, context, 0x4);
                CSF_AT(uint8_t, context, 0x16) = 0;
                const int elementResult =
                    reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                        CSF_AT(void**, serializer, 0)[0x38 / 4])(
                            serializer, nullptr, element, context);
                loopCount = count;
                CSF_AT(uint8_t, context, 0) = elementResult == -1;
            }
            result = static_cast<int>(++index);
            byteOffset += 12;
        }
    }
    else
    {
        if (!CSF_AT(uint8_t, context, 0))
        {
            void* serializer = CSF_AT(void*, context, 0x4);
            result = !reinterpret_cast<bool(__fastcall*)(
                void*, void*, unsigned int*, void*)>(
                    CSF_AT(void**, serializer, 0)[0x78 / 4])(
                        serializer, nullptr, &count, context);
        }
        CSF_AT(uint8_t, context, 0) = static_cast<uint8_t>(result);
        if (result)
        {
            CSF_AT(uint8_t, context, 0) = 1;
            return result;
        }

        if (reuseStorage)
        {
            unsigned int index = 0;
            uint32_t byteOffset = 0;
            while (index < count)
            {
                void* element = reinterpret_cast<void*>(values->field_0004 + byteOffset);
                const bool failed = CSF_AT(uint8_t, context, 0) != 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (!failed)
                {
                    void* serializer = CSF_AT(void*, context, 0x4);
                    CSF_AT(uint8_t, context, 0x15) = reuseStorage;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    CSF_AT(uint8_t, context, 0) =
                        reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                            CSF_AT(void**, serializer, 0)[0x38 / 4])(
                                serializer, nullptr, element, context) == -1;
                }
                result = static_cast<int>(++index);
                byteOffset += 12;
            }
        }
        else
        {
            S_0047ea40_DestroyPointerStorage(values, nullptr);
            result = static_cast<int>(
                TypeRegistry_ReserveVector3Storage(values, nullptr, count));

            if (count)
            {
                alignas(8) uint8_t zeroVectorBytes[0x10] = {};
                unsigned int index = 0;
                uint32_t byteOffset = 0;
                do
                {
                    AiAttackTasks_AppendTangent(
                        values, nullptr,
                        reinterpret_cast<uint32_t*>(zeroVectorBytes));
                    void* element =
                        reinterpret_cast<void*>(values->field_0004 + byteOffset);
                    const bool failed = CSF_AT(uint8_t, context, 0) != 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (!failed)
                    {
                        void* serializer = CSF_AT(void*, context, 0x4);
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        CSF_AT(uint8_t, context, 0) =
                            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                                    serializer, nullptr, element, context) == -1;
                    }
                    result = static_cast<int>(++index);
                    byteOffset += 12;
                } while (index < count);
            }
        }
    }

    uint8_t failed = 1;
    if (!CSF_AT(uint8_t, context, 0))
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        result = reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, context);
        if (result)
            failed = 0;
    }
    CSF_AT(uint8_t, context, 0) = failed;
    return result;
}

HOOK(0x0054D3B0, TypeRegistry_SerializeVector3Array, "auto");
