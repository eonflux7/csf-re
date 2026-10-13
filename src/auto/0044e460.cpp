#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, DynArray_InsertAt, 0x0041E510, (void* self, void* , unsigned int index, uint32_t* value));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, C_0082e664_FindLowerBoundIndex, 0x0041E660, (C_0082e664* self, void* , uint32_t key));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static void __fastcall C_00831210_EnsureEntryCount(
    C_00831210* self, void* /*edx*/, unsigned int requestedCount)
{
    (void)CSF_VTBL(0x0041E5F0);

    const uint32_t initialCount = self->field_000c;
    if (requestedCount <= initialCount)
        return;

    auto* sortedArray = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0x04);
    auto* creationOrderArray = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0x14);

    const uint32_t sortedCapacity = self->field_0008;
    if (requestedCount > sortedCapacity)
    {
        S_0041e5f0_GrowArrayCapacity(
            sortedArray, nullptr,
            static_cast<int>(requestedCount - sortedCapacity));
    }

    const uint32_t creationOrderCapacity = CSF_AT(uint32_t, self, 0x18);
    if (requestedCount > creationOrderCapacity)
    {
        S_0041e5f0_GrowArrayCapacity(
            creationOrderArray, nullptr,
            static_cast<int>(requestedCount - creationOrderCapacity));
    }

    alignas(8) uint8_t insertionBytes[0x8];
    uint32_t& insertionValue = *reinterpret_cast<uint32_t*>(insertionBytes);

    unsigned int remaining = requestedCount - initialCount;
    do
    {
        void* entry = Mem_Alloc(52);
        uint32_t entryAddress = 0;

        if (entry)
        {
            CSF_AT(void**, entry, 0x00) = CSF_VTBL(0x0083124C);
            CSF_AT(int32_t, entry, 0x0C) = -1;
            CSF_AT(int32_t, entry, 0x10) = -1;
            CSF_AT(void*, entry, 0x14) = CSF_VTBL(0x0082D448);
            CSF_AT(int32_t, entry, 0x18) = -1;
            CSF_AT(int32_t, entry, 0x1C) = -1;
            CSF_AT(uint32_t, entry, 0x20) = 0;
            CSF_AT(uint32_t, entry, 0x24) = 0;
            CSF_AT(uint32_t, entry, 0x28) = 0;
            CSF_AT(uint32_t, entry, 0x30) = 0;
            CSF_AT(int32_t, entry, 0x04) = -1;
            CSF_AT(int32_t, entry, 0x08) = -1;
            CSF_AT(uint32_t, entry, 0x2C) = 2139095039;
            entryAddress = reinterpret_cast<uint32_t>(entry);
        }

        insertionValue = entryAddress;
        const uint32_t insertionIndex = static_cast<uint32_t>(
            C_0082e664_FindLowerBoundIndex(
                reinterpret_cast<C_0082e664*>(self), nullptr, entryAddress));

        if (insertionIndex >= self->field_000c)
        {
            if (self->field_000c >= self->field_0008)
            {
                S_0041e5f0_GrowArrayCapacity(
                    sortedArray, nullptr,
                    static_cast<int>(CSF_AT(uint32_t, self, 0x10)));
            }

            const uint32_t count = self->field_000c;
            auto* destination = reinterpret_cast<uint32_t*>(
                self->field_0004 + 4 * count);
            self->field_000c = count + 1;
            *destination = entryAddress;
        }
        else
        {
            DynArray_InsertAt(
                sortedArray, nullptr, insertionIndex, &insertionValue);
        }

        if (self->field_001c >= CSF_AT(uint32_t, self, 0x18))
        {
            S_0041e5f0_GrowArrayCapacity(
                creationOrderArray, nullptr,
                static_cast<int>(CSF_AT(uint32_t, self, 0x20)));
        }

        const uint32_t count = self->field_001c;
        auto* destination = reinterpret_cast<uint32_t*>(
            self->field_0014 + 4 * count);
        self->field_001c = count + 1;
        *destination = entryAddress;
        --remaining;
    }
    while (remaining != 0);
}

HOOK(0x0044E460, C_00831210_EnsureEntryCount, "auto");
