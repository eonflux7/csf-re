#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, DynArray_InsertAt, 0x0041E510, (void* self, void* , unsigned int index, uint32_t* value));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(unsigned int*, __fastcall, C_0085e5c4_FindReferenceInsertionPoint, 0x006917E0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void, __fastcall, SceneLoader_ServiceLoading, 0x0069AF70, (void* self, void* edx));

static char __fastcall C_0085e5c4_InsertUniqueReference(
    C_0085e5c4* self, void* /*edx*/, unsigned int reference)
{
    // Retain the packet's address binding; this address is also the growth callee.
    (void)CSF_VTBL(0x0041E5F0);

    unsigned int* insertionPoint =
        C_0085e5c4_FindReferenceInsertionPoint(self, nullptr, reference);
    if (insertionPoint && *insertionPoint == reference)
        return 1;

    auto* blocks = reinterpret_cast<S_0041e5f0*>(&self->field_0004);
    auto* lastBlock = *reinterpret_cast<S_0041e5f0**>(
        self->field_0004 + 4 * self->field_000c - 4);
    const std::uint32_t lastCount = lastBlock->field_0008;

    if (lastCount == 1000)
    {
        auto* allocation = static_cast<S_0041e5f0*>(Mem_Alloc(0x10u));
        S_0041e5f0* newBlock = nullptr;
        if (allocation)
        {
            allocation->field_0000 = 0;
            allocation->field_0004 = 0;
            allocation->field_0008 = 0;
            allocation->field_000c = 10;
            newBlock = allocation;
        }

        const std::uint32_t capacity = newBlock->field_0004;
        if (capacity < 0x3E8)
            S_0041e5f0_GrowArrayCapacity(
                newBlock, nullptr, static_cast<int>(1000 - capacity));

        if (newBlock->field_0008 >= newBlock->field_0004)
            S_0041e5f0_GrowArrayCapacity(
                newBlock, nullptr, newBlock->field_000c);

        const std::uint32_t newCount = newBlock->field_0008;
        auto* referenceSlot = reinterpret_cast<unsigned int*>(
            newBlock->field_0000 + 4 * newCount);
        newBlock->field_0008 = newCount + 1;
        *referenceSlot = reference;

        if (blocks->field_0008 >= blocks->field_0004)
            S_0041e5f0_GrowArrayCapacity(
                blocks, nullptr, blocks->field_000c);

        const std::uint32_t blockCount = blocks->field_0008;
        auto* blockSlot = reinterpret_cast<S_0041e5f0**>(
            blocks->field_0000 + 4 * blockCount);
        blocks->field_0008 = blockCount + 1;
        *blockSlot = newBlock;

        SceneLoader_ServiceLoading(nullptr, nullptr);
        return 1;
    }

    if (insertionPoint)
    {
        const std::int32_t byteOffset = static_cast<std::int32_t>(
            reinterpret_cast<std::uintptr_t>(insertionPoint) -
            lastBlock->field_0000);
        DynArray_InsertAt(
            lastBlock, nullptr,
            static_cast<unsigned int>(byteOffset >> 2), &reference);
        SceneLoader_ServiceLoading(nullptr, nullptr);
        return 1;
    }

    if (lastCount >= lastBlock->field_0004)
        S_0041e5f0_GrowArrayCapacity(
            lastBlock, nullptr, lastBlock->field_000c);

    const std::uint32_t count = lastBlock->field_0008;
    auto* referenceSlot = reinterpret_cast<unsigned int*>(
        lastBlock->field_0000 + 4 * count);
    lastBlock->field_0008 = count + 1;
    *referenceSlot = reference;

    SceneLoader_ServiceLoading(nullptr, nullptr);
    return 1;
}

HOOK(0x00691B60, C_0085e5c4_InsertUniqueReference, "auto");
