#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, PointerStorage_Reserve, 0x004729C0,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(unsigned int, __fastcall, PointerStorage_Append, 0x00473B70,
         (void* self, void* edx, uint32_t* a3));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10,
         (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790,
         (S_0047ea40* self, void*));

static int __stdcall PointerStorage_SerializeArray(
    S_0047ea40* storage, int archiveAddress)
{
    auto* archive = reinterpret_cast<bool*>(archiveAddress);
    const uint8_t referenceMode = CSF_AT(uint8_t, archive, 0x15);
    uint32_t count = 0;
    int result = 0;

    if (CSF_AT(uint8_t, archive, 0x14))
    {
        const uint32_t begin = storage->field_0004;
        uint32_t loopCount = begin
            ? static_cast<uint32_t>(
                  static_cast<int32_t>(storage->field_0008 - begin) >> 2)
            : 0;
        count = loopCount;

        if (!CSF_AT(uint8_t, archive, 0))
        {
            void* stream = CSF_AT(void*, archive, 4);
            const int status =
                reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, bool*)>(
                    CSF_AT(void**, stream, 0)[0x78 / 4])(
                    stream, nullptr, &count, archive);
            loopCount = count;
            result = static_cast<uint8_t>(status) == 0;
        }

        CSF_AT(uint8_t, archive, 0) = static_cast<uint8_t>(result);
        if (result)
        {
            CSF_AT(uint8_t, archive, 0) = 1;
            return result;
        }

        for (uint32_t index = 0; index < loopCount; ++index)
        {
            const uint32_t elementAddress = storage->field_0004 + 4 * index;
            const bool failed = CSF_AT(uint8_t, archive, 0) != 0;
            result = static_cast<int>(elementAddress);
            CSF_AT(uint32_t, archive, 0x1C) = 0;
            if (!failed)
            {
                CSF_AT(uint8_t, archive, 0x15) = referenceMode;
                CSF_AT(uint8_t, archive, 0x16) = 0;
                result = Archive_SerializeObjectRef(
                    reinterpret_cast<void*>(elementAddress), archive);
                loopCount = count;
            }
        }
    }
    else
    {
        if (!CSF_AT(uint8_t, archive, 0))
        {
            void* stream = CSF_AT(void*, archive, 4);
            const int status =
                reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, bool*)>(
                    CSF_AT(void**, stream, 0)[0x78 / 4])(
                    stream, nullptr, &count, archive);
            result = static_cast<uint8_t>(status) == 0;
        }

        CSF_AT(uint8_t, archive, 0) = static_cast<uint8_t>(result);
        if (result)
        {
            CSF_AT(uint8_t, archive, 0) = 1;
            return result;
        }

        if (referenceMode)
        {
            result = static_cast<int>(count);
            for (uint32_t index = 0; index < count; ++index)
            {
                const uint32_t elementAddress = storage->field_0004 + 4 * index;
                const bool failed = CSF_AT(uint8_t, archive, 0) != 0;
                CSF_AT(uint32_t, archive, 0x1C) = 0;
                if (!failed)
                {
                    CSF_AT(uint8_t, archive, 0x15) = referenceMode;
                    CSF_AT(uint8_t, archive, 0x16) = 0;
                    Archive_SerializeObjectRef(
                        reinterpret_cast<void*>(elementAddress), archive);
                }
                result = static_cast<int>(count);
            }
        }
        else
        {
            S_0047ea40_DestroyPointerStorage(storage, nullptr);
            result = static_cast<int>(
                PointerStorage_Reserve(storage, nullptr, count));

            uint32_t emptyPointer = 0;
            uint32_t index = 0;
            while (index < count)
            {
                PointerStorage_Append(storage, nullptr, &emptyPointer);
                const uint32_t elementAddress = storage->field_0004 + 4 * index;
                const bool failed = CSF_AT(uint8_t, archive, 0) != 0;
                CSF_AT(uint32_t, archive, 0x1C) = 0;
                if (!failed)
                {
                    CSF_AT(uint8_t, archive, 0x15) = 0;
                    CSF_AT(uint8_t, archive, 0x16) = 0;
                    Archive_SerializeObjectRef(
                        reinterpret_cast<void*>(elementAddress), archive);
                }
                ++index;
                result = static_cast<int>(index);
            }
        }
    }

    uint8_t failed = 1;
    if (!CSF_AT(uint8_t, archive, 0))
    {
        void* stream = CSF_AT(void*, archive, 4);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, bool*)>(
            CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, archive);
        failed = static_cast<uint8_t>(result) == 0;
    }
    CSF_AT(uint8_t, archive, 0) = failed;
    return result;
}

HOOK(0x00473BE0, PointerStorage_SerializeArray, "auto");
