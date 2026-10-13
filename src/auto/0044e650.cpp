#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, SerializeObjectRefWithFlags, 0x00405BC0, (int a1, S_00691860* a2, int a3, char a4, char a5));
CSF_FUNC(int, __fastcall, EdgeLatch_Poll, 0x0040FD40, (void* self, void* , int a2));
CSF_FUNC(int, __fastcall, DynArray_InsertAt, 0x0041E510, (void* self, void* , unsigned int index, uint32_t* value));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, C_0082e664_FindLowerBoundIndex, 0x0041E660, (C_0082e664* self, void* , uint32_t key));
CSF_FUNC(int, __fastcall, C_008311f0_FindReferenceIndex, 0x0044D9B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint8_t*, __fastcall, EraseReferenceAt, 0x0044E9E0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __fastcall, CharacterDefs_AssignEmbeddedMember, 0x006CAF70, (void* self, void* , void* source));

static int __fastcall C_008311f0_SerializeReferences(
    C_008311f0* self, void* /*edx*/, int archiveAddress)
{
    auto* archive = reinterpret_cast<uint8_t*>(archiveAddress);
    uint8_t failed;
    if (CSF_AT(uint8_t, archive, 0) != 0)
    {
        failed = 0;
    }
    else
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        bool started =
            reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, stream, 0)[0x80 / 4])(
                    stream, nullptr, 0, archiveAddress);
        failed = started ? 0 : 1;
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;

    alignas(8) uint8_t referencesBytes[0x24];
    CSF_AT(uint32_t, referencesBytes, 0x00) = 0;
    CSF_AT(uint32_t, referencesBytes, 0x04) = 0;
    CSF_AT(uint32_t, referencesBytes, 0x08) = 0;
    CSF_AT(uint32_t, referencesBytes, 0x0C) = 10;

    if (CSF_AT(uint8_t, archive, 0x14))
    {
        CharacterDefs_AssignEmbeddedMember(
            referencesBytes, nullptr, reinterpret_cast<uint8_t*>(self) + 4);

        const uint32_t excludedCount = self->field_001c;
        for (uint32_t i = 0; i < excludedCount; ++i)
        {
            const int index = C_008311f0_FindReferenceIndex(
                self, nullptr,
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 4 * i));
            if (index != -1)
            {
                CSF_AT(uint32_t,
                    reinterpret_cast<void*>(CSF_AT(uint32_t, referencesBytes, 0)),
                    4u * static_cast<uint32_t>(index)) = 0;
            }
        }

        for (int32_t i =
                 static_cast<int32_t>(CSF_AT(uint32_t, referencesBytes, 0x08) - 1);
             i >= 0; --i)
        {
            if (CSF_AT(uint32_t,
                    reinterpret_cast<void*>(CSF_AT(uint32_t, referencesBytes, 0)),
                    4u * static_cast<uint32_t>(i)) == 0)
            {
                EraseReferenceAt(
                    referencesBytes, nullptr, reinterpret_cast<void*>(i));
            }
        }
    }

    alignas(8) uint8_t countBytes[4];
    uint32_t& count = CSF_AT(uint32_t, countBytes, 0);
    count = CSF_AT(uint32_t, referencesBytes, 0x08);
    uint32_t serializedCount = count;

    const bool canSerializeCount = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerializeCount)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        EdgeLatch_Poll(archive, nullptr, reinterpret_cast<int>(&count));
        serializedCount = count;
    }

    if (CSF_AT(uint8_t, archive, 0x14))
    {
        for (uint32_t i = 0; i < serializedCount; )
        {
            auto* reference = reinterpret_cast<S_00691860*>(
                CSF_AT(uint32_t, referencesBytes, 0) + 4 * i);
            ++i;
            SerializeObjectRefWithFlags(archiveAddress, reference, 0, 1, 0);
            serializedCount = count;
        }
    }
    else
    {
        uint32_t originalCount = self->field_000c;
        auto* referenceArray = reinterpret_cast<uint8_t*>(self) + 4;

        if (serializedCount > originalCount)
        {
            if (serializedCount > self->field_0008)
            {
                S_0041e5f0_GrowArrayCapacity(
                    reinterpret_cast<S_0041e5f0*>(referenceArray), nullptr,
                    static_cast<int>(serializedCount - self->field_0008));
                serializedCount = count;
            }

            alignas(8) uint8_t referenceBytes[4];
            uint32_t& reference = CSF_AT(uint32_t, referenceBytes, 0);
            for (uint32_t i = originalCount; i < serializedCount; )
            {
                void* object = Mem_Alloc(52);
                if (object)
                {
                    CSF_AT(void**, object, 0) = CSF_VTBL(0x0083124C);
                    CSF_AT(int, object, 0x0C) = -1;
                    CSF_AT(int, object, 0x10) = -1;
                    CSF_AT(void*, object, 0x14) = CSF_VTBL(0x0082D448);
                    CSF_AT(int, object, 0x18) = -1;
                    CSF_AT(int, object, 0x1C) = -1;
                    CSF_AT(uint32_t, object, 0x20) = 0;
                    CSF_AT(uint32_t, object, 0x24) = 0;
                    CSF_AT(uint32_t, object, 0x28) = 0;
                    CSF_AT(int, object, 0x04) = -1;
                    CSF_AT(int, object, 0x08) = -1;
                    CSF_AT(float, object, 0x2C) = FLT_MAX;
                    CSF_AT(uint32_t, object, 0x30) = 0;
                    reference = reinterpret_cast<uint32_t>(object);
                }
                else
                {
                    reference = 0;
                }

                const uint32_t index = static_cast<uint32_t>(
                    C_0082e664_FindLowerBoundIndex(
                        reinterpret_cast<C_0082e664*>(self), nullptr, reference));
                if (index < self->field_000c)
                {
                    DynArray_InsertAt(referenceArray, nullptr, index, &reference);
                }
                else
                {
                    const uint32_t capacity = self->field_0008;
                    if (self->field_000c >= capacity)
                    {
                        const uint32_t growth = CSF_AT(uint32_t, self, 0x10);
                        void* replacement = Mem_Alloc(4u * (growth + capacity));
                        const uint32_t used = self->field_000c;
                        if (used)
                        {
                            std::memcpy(replacement,
                                reinterpret_cast<void*>(self->field_0004), 4u * used);
                        }
                        self->field_0008 += growth;
                        if (self->field_0004)
                        {
                            Mem_Free(reinterpret_cast<void*>(self->field_0004));
                            self->field_0004 = 0;
                        }
                        self->field_0004 = reinterpret_cast<uint32_t>(replacement);
                    }

                    const uint32_t used = self->field_000c;
                    auto* destination = reinterpret_cast<uint32_t*>(
                        self->field_0004 + 4 * used);
                    self->field_000c = used + 1;
                    *destination = reference;
                }
                ++i;
                serializedCount = count;
            }
        }

        for (uint32_t i = 0; i < serializedCount; ++i)
        {
            void* reference = reinterpret_cast<void*>(self->field_0004 + 4 * i);
            const bool canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
            CSF_AT(uint32_t, archive, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, archive, 0x15) = 1;
                CSF_AT(uint8_t, archive, 0x16) = 0;
                Archive_SerializeObjectRef(reference, reinterpret_cast<bool*>(archive));
                serializedCount = count;
            }
        }

        const uint32_t totalCount = self->field_000c;
        const uint32_t surplus = totalCount - serializedCount;
        self->field_001c = 0;

        if (static_cast<int32_t>(surplus) > 0)
        {
            const uint32_t capacity = CSF_AT(uint32_t, self, 0x18);
            if (surplus > capacity)
            {
                const uint32_t growth = surplus - capacity;
                void* replacement = Mem_Alloc(4u * surplus);
                const uint32_t used = self->field_001c;
                if (used)
                {
                    std::memcpy(replacement,
                        reinterpret_cast<void*>(self->field_0014), 4u * used);
                }
                CSF_AT(uint32_t, self, 0x18) += growth;
                if (self->field_0014)
                {
                    Mem_Free(reinterpret_cast<void*>(self->field_0014));
                    self->field_0014 = 0;
                }
                serializedCount = count;
                self->field_0014 = reinterpret_cast<uint32_t>(replacement);
            }

            for (uint32_t i = serializedCount; i < totalCount; ++i)
            {
                const uint32_t capacity = CSF_AT(uint32_t, self, 0x18);
                auto* source = reinterpret_cast<uint32_t*>(
                    self->field_0004 + 4 * i);
                if (self->field_001c >= capacity)
                {
                    const uint32_t growth = CSF_AT(uint32_t, self, 0x20);
                    void* replacement = Mem_Alloc(4u * (growth + capacity));
                    const uint32_t used = self->field_001c;
                    if (used)
                    {
                        std::memcpy(replacement,
                            reinterpret_cast<void*>(self->field_0014), 4u * used);
                    }
                    CSF_AT(uint32_t, self, 0x18) += growth;
                    if (self->field_0014)
                    {
                        Mem_Free(reinterpret_cast<void*>(self->field_0014));
                        self->field_0014 = 0;
                    }
                    self->field_0014 = reinterpret_cast<uint32_t>(replacement);
                }

                const uint32_t used = self->field_001c;
                auto* destination = reinterpret_cast<uint32_t*>(
                    self->field_0014 + 4 * used);
                self->field_001c = used + 1;
                *destination = *source;
            }
        }
    }

    const bool canFinish = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    int result = 1;
    if (canFinish)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(
                    stream, nullptr, archiveAddress))
        {
            result = 0;
        }
    }
    CSF_AT(uint8_t, archive, 0) = static_cast<uint8_t>(result);

    if (CSF_AT(uint32_t, referencesBytes, 0))
    {
        Mem_Free(reinterpret_cast<void*>(
            CSF_AT(uint32_t, referencesBytes, 0)));
    }
    return result;
}

HOOK(0x0044E650, C_008311f0_SerializeReferences, "auto");
