#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Serialization_ClearEntries, 0x0044C2F0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, Serialization_ConstructEntry, 0x0044C3D0,
         (int a1, int a2));
CSF_FUNC(int, __fastcall, Serialization_SerializeEntry, 0x0044C600,
         (void* self, void* edx, void* a2));
CSF_FUNC(unsigned int, __fastcall, Serialization_ReserveEntries, 0x0044CB20,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, Serialization_AppendEntry, 0x0044CDB0,
         (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static unsigned int __stdcall Serialization_SerializeRecordArray(
    S_0044cd10* entries, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t existingEntries = CSF_AT(uint8_t, context, 0x15);
    uint32_t count = 0;
    unsigned int result = 0;

    if (CSF_AT(uint8_t, context, 0x14))
    {
        const uint32_t begin = entries->field_0004;
        result = begin
            ? static_cast<unsigned int>(
                  static_cast<int32_t>(entries->field_0008 - begin) / 100)
            : 0;
        count = result;

        bool failed = false;
        if (!CSF_AT(uint8_t, context, 0))
        {
            void* stream = CSF_AT(void*, context, 0x04);
            failed = !reinterpret_cast<bool(__fastcall*)(
                void*, void*, uint32_t*, void*)>(
                    CSF_AT(void**, stream, 0)[0x78 / 4])(
                        stream, nullptr, &count, context);
            result = count;
        }
        CSF_AT(uint8_t, context, 0) = static_cast<uint8_t>(failed);
        if (failed)
            return result;

        uint32_t index = 0;
        uint32_t byteOffset = 0;
        while (index < result)
        {
            void* entry = reinterpret_cast<void*>(
                entries->field_0004 + byteOffset);
            const bool blocked = CSF_AT(uint8_t, context, 0) != 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!blocked)
            {
                CSF_AT(uint8_t, context, 0x15) = existingEntries;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Serialization_SerializeEntry(entry, nullptr, context);
                result = count;
            }
            byteOffset += 100;
            ++index;
        }
    }
    else
    {
        bool failed = false;
        if (!CSF_AT(uint8_t, context, 0))
        {
            void* stream = CSF_AT(void*, context, 0x04);
            failed = !reinterpret_cast<bool(__fastcall*)(
                void*, void*, uint32_t*, void*)>(
                    CSF_AT(void**, stream, 0)[0x78 / 4])(
                        stream, nullptr, &count, context);
        }
        result = static_cast<unsigned int>(failed);
        CSF_AT(uint8_t, context, 0) = static_cast<uint8_t>(failed);
        if (failed)
            return result;

        if (existingEntries)
        {
            uint32_t index = 0;
            uint32_t byteOffset = 0;
            while (index < count)
            {
                void* entry = reinterpret_cast<void*>(
                    entries->field_0004 + byteOffset);
                const bool blocked = CSF_AT(uint8_t, context, 0) != 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (!blocked)
                {
                    CSF_AT(uint8_t, context, 0x15) = existingEntries;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Serialization_SerializeEntry(entry, nullptr, context);
                }
                result = index + 1;
                byteOffset += 100;
                ++index;
            }
        }
        else
        {
            Serialization_ClearEntries(entries, nullptr);
            result = Serialization_ReserveEntries(entries, nullptr, count);

            alignas(8) uint8_t entryBytes[0x68];
            uint32_t index = 0;
            uint32_t byteOffset = 0;
            while (index < count)
            {
                Serialization_ConstructEntry(
                    reinterpret_cast<int>(entryBytes), 0);
                Serialization_AppendEntry(
                    entries, nullptr, reinterpret_cast<int>(entryBytes));

                void* entry = reinterpret_cast<void*>(
                    entries->field_0004 + byteOffset);
                const bool blocked = CSF_AT(uint8_t, context, 0) != 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (!blocked)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Serialization_SerializeEntry(entry, nullptr, context);
                }

                if (CSF_AT(uint32_t, entryBytes, 0x58) >= 0x10)
                {
                    void* block = CSF_AT(void*, entryBytes, 0x44);
                    if (block)
                        Mem_FreeDebug(block);
                }

                result = index + 1;
                byteOffset += 100;
                const bool moreEntries = result < count;
                CSF_AT(uint32_t, entryBytes, 0x58) = 15;
                CSF_AT(uint32_t, entryBytes, 0x54) = 0;
                CSF_AT(uint8_t, entryBytes, 0x44) = 0;
                CSF_AT(void**, entryBytes, 0x10) = CSF_VTBL(0x0082D410);
                ++index;
                if (!moreEntries)
                    break;
            }
        }
    }

    uint8_t failed = 0;
    if (CSF_AT(uint8_t, context, 0))
    {
        failed = 1;
    }
    else
    {
        void* stream = CSF_AT(void*, context, 0x04);
        result = reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, stream, 0)[0x7C / 4])(
                stream, nullptr, context);
        if (!result)
            failed = 1;
    }
    CSF_AT(uint8_t, context, 0) = failed;
    return result;
}

HOOK(0x0044CE40, Serialization_SerializeRecordArray, "auto");
