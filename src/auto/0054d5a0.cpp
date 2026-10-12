#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_AppendSlotOrGrow, 0x0047EE50, (void* self, void* , uint32_t* pointerArg));
CSF_FUNC(unsigned int, __fastcall, TypeRegistry_ReservePointerSlots, 0x0054D1B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790, (S_0047ea40* self, void*));

static int __stdcall TypeRegistry_SerializePointerVector(S_0047ea40* storage, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t originalMode = CSF_AT(uint8_t, context, 0x15);

    alignas(8) uint8_t countBytes[0x4] = {};
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);
    int result = 0;

    if (CSF_AT(uint8_t, context, 0x14))
    {
        uint32_t loopCount = 0;
        const uint32_t begin = storage->field_0004;
        if (begin)
            loopCount = static_cast<int32_t>(storage->field_0008 - begin) >> 2;
        count = loopCount;

        if (!CSF_AT(uint8_t, context, 0x00))
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            const bool success =
                reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, void*)>(
                    CSF_AT(void**, serializer, 0)[0x78 / 4])(
                        serializer, nullptr, &count, context);
            loopCount = count;
            result = !success;
        }
        CSF_AT(uint8_t, context, 0x00) = static_cast<uint8_t>(result);
        if (result)
        {
            CSF_AT(uint8_t, context, 0x00) = 1;
            return result;
        }

        for (uint32_t index = 0; index < loopCount; ++index)
        {
            const uint32_t slotAddress = storage->field_0004 + 4 * index;
            const bool failed = CSF_AT(uint8_t, context, 0x00) != 0;
            result = static_cast<int32_t>(slotAddress);
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!failed)
            {
                CSF_AT(uint8_t, context, 0x15) = originalMode;
                void* serializer = CSF_AT(void*, context, 0x04);
                CSF_AT(uint8_t, context, 0x16) = 0;
                const int itemResult =
                    reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t, void*)>(
                        CSF_AT(void**, serializer, 0)[0x54 / 4])(
                            serializer, nullptr, slotAddress, context);
                loopCount = count;
                result = itemResult == -1;
                CSF_AT(uint8_t, context, 0x00) = static_cast<uint8_t>(result);
            }
        }
    }
    else
    {
        if (!CSF_AT(uint8_t, context, 0x00))
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            result =
                !reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, void*)>(
                    CSF_AT(void**, serializer, 0)[0x78 / 4])(
                        serializer, nullptr, &count, context);
        }
        CSF_AT(uint8_t, context, 0x00) = static_cast<uint8_t>(result);
        if (result)
        {
            CSF_AT(uint8_t, context, 0x00) = 1;
            return result;
        }

        if (originalMode)
        {
            result = static_cast<int32_t>(count);
            for (uint32_t index = 0; index < count; ++index)
            {
                const uint32_t slotAddress = storage->field_0004 + 4 * index;
                const bool failed = CSF_AT(uint8_t, context, 0x00) != 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (!failed)
                {
                    CSF_AT(uint8_t, context, 0x15) = originalMode;
                    void* serializer = CSF_AT(void*, context, 0x04);
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    const int itemResult =
                        reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t, void*)>(
                            CSF_AT(void**, serializer, 0)[0x54 / 4])(
                                serializer, nullptr, slotAddress, context);
                    CSF_AT(uint8_t, context, 0x00) = itemResult == -1;
                }
                result = static_cast<int32_t>(count);
            }
        }
        else
        {
            S_0047ea40_DestroyPointerStorage(storage, nullptr);
            result = static_cast<int32_t>(
                TypeRegistry_ReservePointerSlots(storage, nullptr, count));

            if (count)
            {
                uint32_t emptySlot = 0;
                uint32_t index = 0;
                do
                {
                    AiAttackTasks_AppendSlotOrGrow(storage, nullptr, &emptySlot);
                    const uint32_t slotAddress = storage->field_0004 + 4 * index;
                    const bool failed = CSF_AT(uint8_t, context, 0x00) != 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (!failed)
                    {
                        void* serializer = CSF_AT(void*, context, 0x04);
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        const int itemResult =
                            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t, void*)>(
                                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                                    serializer, nullptr, slotAddress, context);
                        CSF_AT(uint8_t, context, 0x00) = itemResult == -1;
                    }
                    ++index;
                    result = static_cast<int32_t>(index);
                } while (index < count);
            }
        }
    }

    uint8_t failed = 1;
    if (!CSF_AT(uint8_t, context, 0x00))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, context);
        if (static_cast<uint8_t>(result))
            failed = 0;
    }
    CSF_AT(uint8_t, context, 0x00) = failed;
    return result;
}

HOOK(0x0054D5A0, TypeRegistry_SerializePointerVector, "auto");
