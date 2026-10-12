#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, IntVector_FillUninitialized, 0x0046BFB0, (std::uint32_t* destination, std::uint32_t count, const std::uint32_t* source, void* context));
CSF_FUNC(unsigned int, __fastcall, IntVector_InsertN, 0x0046C940, (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

static unsigned int __fastcall C_00840724_CompleteNavigationLoad(
    C_00840724* self, void* /*edx*/)
{
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* navigation = reinterpret_cast<void*>(
        (reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, accessor, 0)[0x14 / 4]))(accessor, nullptr));

    (reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, navigation, 0)[0x2C / 4]))(navigation, nullptr);

    alignas(8) std::uint8_t identifierBytes[0x10];
    auto* identifier = reinterpret_cast<std::uint32_t*>(identifierBytes);

    unsigned int recordIndex = 0;
    std::uint32_t recordOffset = 0;
    std::uint32_t vectorOffset = 0;

    for (;;)
    {
        void* records = reinterpret_cast<void*>(self->field_0004);
        std::uint32_t begin = CSF_AT(std::uint32_t, records, 0x04);
        if (!begin)
            return 0;

        unsigned int recordCount = static_cast<unsigned int>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, records, 0x08) - begin) / 96);
        if (recordIndex >= recordCount)
            return recordCount;

        auto* record = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, records, 0x04)) + recordOffset;
        std::uint32_t namesBegin = CSF_AT(std::uint32_t, record, 0x24);
        unsigned int nameCount = 0;
        if (namesBegin)
        {
            std::int32_t nameBytes = static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, record, 0x28) - namesBegin);
            // Signed division by the 44-byte name-entry stride.
            std::int32_t scaledCount = static_cast<std::int32_t>(
                (static_cast<std::int64_t>(nameBytes) * 0x2E8BA2E9) >> 32) >> 3;
            nameCount = static_cast<unsigned int>(scaledCount);
            nameCount += nameCount >> 31;
        }

        auto* destination = reinterpret_cast<std::uint8_t*>(
            self->field_000c) + vectorOffset;
        void* oldStorage = CSF_AT(void*, destination, 0x04);
        if (oldStorage)
            Mem_FreeDebug(oldStorage);
        CSF_AT(std::uint32_t, destination, 0x04) = 0;
        CSF_AT(std::uint32_t, destination, 0x08) = 0;
        CSF_AT(std::uint32_t, destination, 0x0C) = 0;

        std::uint32_t nameOffset = 0;
        for (unsigned int remaining = nameCount; remaining; --remaining)
        {
            records = reinterpret_cast<void*>(self->field_0004);
            record = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, records, 0x04)) + recordOffset;
            auto* nameEntry = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, record, 0x24)) + nameOffset;

            const char* name;
            if (CSF_AT(std::uint32_t, nameEntry, 0x18) < 0x10)
                name = reinterpret_cast<const char*>(nameEntry + 4);
            else
                name = CSF_AT(const char*, nameEntry, 0x04);

            int resolvedId = (reinterpret_cast<int(__fastcall*)(
                void*, void*, const char*)>(
                    CSF_AT(void**, navigation, 0)[0x18 / 4]))(
                        navigation, nullptr, name);
            *identifier = static_cast<std::uint32_t>(resolvedId);

            if (resolvedId != -1)
            {
                std::uint32_t storageBegin =
                    CSF_AT(std::uint32_t, destination, 0x04);
                std::uint32_t storageEnd =
                    CSF_AT(std::uint32_t, destination, 0x08);
                std::uint32_t cursor = storageBegin;

                while (cursor != storageEnd)
                {
                    if (CSF_AT(int, reinterpret_cast<void*>(cursor), 0) == resolvedId)
                        break;
                    cursor += 4;
                }

                if (cursor == storageEnd)
                {
                    unsigned int size = storageBegin
                        ? static_cast<unsigned int>(
                            static_cast<std::int32_t>(
                                storageEnd - storageBegin) >> 2)
                        : 0;

                    if (storageBegin &&
                        size < static_cast<unsigned int>(
                            static_cast<std::int32_t>(
                                CSF_AT(std::uint32_t, destination, 0x0C) -
                                storageBegin) >> 2))
                    {
                        IntVector_FillUninitialized(
                            reinterpret_cast<std::uint32_t*>(storageEnd),
                            1, identifier, destination);
                        CSF_AT(std::uint32_t, destination, 0x08) =
                            storageEnd + 4;
                    }
                    else
                    {
                        IntVector_InsertN(
                            destination, nullptr,
                            reinterpret_cast<char*>(storageEnd), 1u, identifier);
                    }
                }
            }
            nameOffset += 44;
        }

        ++recordIndex;
        vectorOffset += 16;
        recordOffset += 96;
    }
}

HOOK(0x0051A550, C_00840724_CompleteNavigationLoad, "auto");
