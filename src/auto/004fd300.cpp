#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, IntArray_Reserve, 0x004FD1A0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(unsigned int, __fastcall, IntArray_PushBack, 0x004FD290, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790, (S_0047ea40* self, void*));

static int __stdcall CommandoAnims_SerializeIntArray(S_0047ea40* array, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t savedMode = CSF_AT(uint8_t, context, 0x15);
    uint32_t count = 0;
    int result = 0;

    if (CSF_AT(uint8_t, context, 0x14))
    {
        const uint32_t begin = array->field_0004;
        uint32_t limit = begin
            ? static_cast<uint32_t>(static_cast<int32_t>(array->field_0008 - begin) >> 2)
            : 0;
        count = limit;

        if (!CSF_AT(uint8_t, context, 0))
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            const bool started =
                ((bool(__fastcall*)(void*, void*, uint32_t*, void*))
                    CSF_AT(void**, serializer, 0)[0x78 / 4])(
                        serializer, nullptr, &count, context);
            limit = count;
            result = !started;
        }
        CSF_AT(uint8_t, context, 0) = static_cast<uint8_t>(result);
        if (result)
        {
            CSF_AT(uint8_t, context, 0) = 1;
            return result;
        }

        for (uint32_t index = 0; index < limit; ++index)
        {
            const uint32_t beginAddress = array->field_0004;
            const bool canProcess = !CSF_AT(uint8_t, context, 0);
            result = static_cast<int32_t>(beginAddress + 4 * index);
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canProcess)
            {
                CSF_AT(uint8_t, context, 0x15) = savedMode;
                void* serializer = CSF_AT(void*, context, 0x04);
                CSF_AT(uint8_t, context, 0x16) = 0;
                const int elementResult =
                    ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                        CSF_AT(void**, serializer, 0)[0x48 / 4])(
                            serializer, nullptr,
                            reinterpret_cast<uint32_t*>(result), context);
                limit = count;
                result = elementResult == -1;
                CSF_AT(uint8_t, context, 0) = static_cast<uint8_t>(result);
            }
        }
    }
    else
    {
        if (!CSF_AT(uint8_t, context, 0))
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            result =
                !((bool(__fastcall*)(void*, void*, uint32_t*, void*))
                    CSF_AT(void**, serializer, 0)[0x78 / 4])(
                        serializer, nullptr, &count, context);
        }
        CSF_AT(uint8_t, context, 0) = static_cast<uint8_t>(result);
        if (result)
        {
            CSF_AT(uint8_t, context, 0) = 1;
            return result;
        }

        if (savedMode)
        {
            result = static_cast<int32_t>(count);
            for (uint32_t index = 0; index < count; ++index)
            {
                const uint32_t beginAddress = array->field_0004;
                const bool canProcess = !CSF_AT(uint8_t, context, 0);
                uint32_t* element =
                    reinterpret_cast<uint32_t*>(beginAddress + 4 * index);
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(uint8_t, context, 0x15) = savedMode;
                    void* serializer = CSF_AT(void*, context, 0x04);
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    const int elementResult =
                        ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                            CSF_AT(void**, serializer, 0)[0x48 / 4])(
                                serializer, nullptr, element, context);
                    CSF_AT(uint8_t, context, 0) = elementResult == -1;
                }
                result = static_cast<int32_t>(count);
            }
        }
        else
        {
            S_0047ea40_DestroyPointerStorage(array, nullptr);
            result = static_cast<int32_t>(IntArray_Reserve(array, nullptr, count));
            uint32_t index = 0;
            if (count)
            {
                uint32_t emptyElement = 0;
                do
                {
                    IntArray_PushBack(array, nullptr, &emptyElement);
                    const uint32_t beginAddress = array->field_0004;
                    const bool canProcess = !CSF_AT(uint8_t, context, 0);
                    uint32_t* element =
                        reinterpret_cast<uint32_t*>(beginAddress + 4 * index);
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (canProcess)
                    {
                        void* serializer = CSF_AT(void*, context, 0x04);
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        const int elementResult =
                            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                                    serializer, nullptr, element, context);
                        CSF_AT(uint8_t, context, 0) = elementResult == -1;
                    }
                    ++index;
                    result = static_cast<int32_t>(index);
                } while (index < count);
            }
        }
    }

    uint8_t failed = 1;
    if (!CSF_AT(uint8_t, context, 0))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        result =
            ((int(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, context);
        if (static_cast<uint8_t>(result))
            failed = 0;
    }
    CSF_AT(uint8_t, context, 0) = failed;
    return result;
}

HOOK(0x004FD300, CommandoAnims_SerializeIntArray, "auto");
