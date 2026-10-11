#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_40FD40, 0x0040FD40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_41E510, 0x0041E510, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_41E660, 0x0041E660, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, sub_44D9B0, 0x0044D9B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint8_t*, __fastcall, sub_44E9E0, 0x0044E9E0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(unsigned int*, __fastcall, sub_6AF390, 0x006AF390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6CAF70, 0x006CAF70, (void* self, void* edx, int a2));

static int __fastcall C_0082e664_SerializeObjectList(
    C_0082e664* self, void* /*edx*/, int archiveAddress)
{
    auto* archive = reinterpret_cast<uint8_t*>(archiveAddress);

    // The packet also identifies the direct GrowButtonArray target as a vtable.
    (void)CSF_VTBL(0x0041E5F0);

    uint8_t failed = 0;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, stream, 0)[0x80 / 4])(
                stream, nullptr, 0, archiveAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;

    // Temporary array, with room for the complete original stack object.
    uint32_t snapshot[9] = {};
    snapshot[3] = 10;

    if (CSF_AT(uint8_t, archive, 0x14))
    {
        sub_6CAF70(snapshot, nullptr,
                   reinterpret_cast<int>(&self->field_0004));

        const uint32_t excludedCount = self->field_001c;
        for (uint32_t i = 0; i < excludedCount; ++i)
        {
            const int index = sub_44D9B0(
                self, nullptr,
                reinterpret_cast<uint32_t*>(self->field_0014)[i]);
            if (index != -1)
                reinterpret_cast<uint32_t*>(snapshot[0])[index] = 0;
        }

        for (int32_t i = static_cast<int32_t>(snapshot[2] - 1); i >= 0; --i)
        {
            if (reinterpret_cast<uint32_t*>(snapshot[0])[i] == 0)
            {
                sub_44E9E0(snapshot, nullptr,
                    reinterpret_cast<void*>(static_cast<uint32_t>(i)));
            }
        }
    }

    uint32_t serializedCount = snapshot[2];
    uint32_t loopCount = serializedCount;
    const bool canSerializeCount = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerializeCount)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        sub_40FD40(archive, nullptr, reinterpret_cast<int>(&serializedCount));
        loopCount = serializedCount;
    }

    if (CSF_AT(uint8_t, archive, 0x14))
    {
        for (uint32_t i = 0; i < loopCount; ++i)
        {
            void* entry = reinterpret_cast<uint32_t*>(snapshot[0]) + i;
            const bool canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
            CSF_AT(uint32_t, archive, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, archive, 0x15) = 1;
                CSF_AT(uint8_t, archive, 0x16) = 0;
                Archive_SerializeObjectRef(
                    entry, reinterpret_cast<bool*>(archive));
                loopCount = serializedCount;
            }
        }
    }
    else
    {
        uint32_t oldCount = self->field_000c;
        if (loopCount > oldCount)
        {
            const uint32_t capacity = self->field_0008;
            if (loopCount > capacity)
            {
                GrowButtonArray(&self->field_0004, nullptr,
                    static_cast<int>(loopCount - capacity));
                loopCount = serializedCount;
            }

            for (uint32_t i = oldCount; i < loopCount; ++i)
            {
                void* object = Mem_Alloc(104);
                uint32_t objectAddress = reinterpret_cast<uint32_t>(object);
                if (object)
                {
                    CSF_AT(void**, object, 0) = CSF_VTBL(0x0082E684);
                    sub_6AF390(static_cast<uint8_t*>(object) + 4, nullptr);
                }
                else
                {
                    objectAddress = 0;
                }

                const uint32_t insertionIndex = static_cast<uint32_t>(
                    sub_41E660(self, nullptr, objectAddress));
                if (insertionIndex >= self->field_000c)
                {
                    const uint32_t currentCapacity = self->field_0008;
                    if (self->field_000c >= currentCapacity)
                    {
                        const uint32_t growth = CSF_AT(uint32_t, self, 0x10);
                        void* newData = Mem_Alloc(4 * (growth + currentCapacity));
                        const uint32_t countToCopy = self->field_000c;
                        if (countToCopy)
                        {
                            std::memcpy(newData,
                                reinterpret_cast<void*>(self->field_0004),
                                4 * countToCopy);
                        }
                        self->field_0008 += growth;
                        if (self->field_0004)
                        {
                            Mem_Free(reinterpret_cast<void*>(self->field_0004));
                            self->field_0004 = 0;
                        }
                        self->field_0004 = reinterpret_cast<uint32_t>(newData);
                    }

                    const uint32_t count = self->field_000c;
                    auto* destination =
                        reinterpret_cast<uint32_t*>(self->field_0004) + count;
                    self->field_000c = count + 1;
                    *destination = objectAddress;
                }
                else
                {
                    sub_41E510(&self->field_0004, nullptr,
                        static_cast<int>(insertionIndex), &objectAddress);
                }
                loopCount = serializedCount;
            }
        }

        for (uint32_t i = 0; i < loopCount; ++i)
        {
            void* entry = reinterpret_cast<uint32_t*>(self->field_0004) + i;
            const bool canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
            CSF_AT(uint32_t, archive, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, archive, 0x15) = 1;
                CSF_AT(uint8_t, archive, 0x16) = 0;
                Archive_SerializeObjectRef(
                    entry, reinterpret_cast<bool*>(archive));
                loopCount = serializedCount;
            }
        }

        const uint32_t totalCount = self->field_000c;
        const uint32_t remainingCount = totalCount - loopCount;
        self->field_001c = 0;
        if (static_cast<int32_t>(remainingCount) > 0)
        {
            const uint32_t capacity = CSF_AT(uint32_t, self, 0x18);
            if (remainingCount > capacity)
            {
                const uint32_t growth = remainingCount - capacity;
                void* newData = Mem_Alloc(4 * remainingCount);
                const uint32_t countToCopy = self->field_001c;
                if (countToCopy)
                {
                    std::memcpy(newData,
                        reinterpret_cast<void*>(self->field_0014),
                        4 * countToCopy);
                }
                CSF_AT(uint32_t, self, 0x18) += growth;
                if (self->field_0014)
                {
                    Mem_Free(reinterpret_cast<void*>(self->field_0014));
                    self->field_0014 = 0;
                }
                self->field_0014 = reinterpret_cast<uint32_t>(newData);
                loopCount = serializedCount;
            }

            for (uint32_t i = loopCount; i < totalCount; ++i)
            {
                const uint32_t capacityNow = CSF_AT(uint32_t, self, 0x18);
                auto* source =
                    reinterpret_cast<uint32_t*>(self->field_0004) + i;
                if (self->field_001c >= capacityNow)
                {
                    const uint32_t growth = CSF_AT(uint32_t, self, 0x20);
                    void* newData = Mem_Alloc(4 * (growth + capacityNow));
                    const uint32_t countToCopy = self->field_001c;
                    if (countToCopy)
                    {
                        std::memcpy(newData,
                            reinterpret_cast<void*>(self->field_0014),
                            4 * countToCopy);
                    }
                    CSF_AT(uint32_t, self, 0x18) += growth;
                    if (self->field_0014)
                    {
                        Mem_Free(reinterpret_cast<void*>(self->field_0014));
                        self->field_0014 = 0;
                    }
                    self->field_0014 = reinterpret_cast<uint32_t>(newData);
                }

                const uint32_t count = self->field_001c;
                auto* destination =
                    reinterpret_cast<uint32_t*>(self->field_0014) + count;
                self->field_001c = count + 1;
                *destination = *source;
            }
        }
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    int result = 1;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(
                    stream, nullptr, archiveAddress))
        {
            result = 0;
        }
    }
    CSF_AT(uint8_t, archive, 0) = static_cast<uint8_t>(result);

    if (snapshot[0])
        Mem_Free(reinterpret_cast<void*>(snapshot[0]));
    return result;
}

HOOK(0x0041F330, C_0082e664_SerializeObjectList, "auto");
