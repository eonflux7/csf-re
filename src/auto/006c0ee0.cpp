#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static char __cdecl GfxTexDict_ClumpsShareResource(int firstClump, int secondClump)
{
    alignas(8) uint8_t firstArray[0x10];
    alignas(8) uint8_t secondArray[0x10];

    CSF_AT(uint32_t, firstArray, 0x0C) = 10;
    CSF_AT(uint32_t, secondArray, 0x0C) = 10;
    char matched = 0;
    CSF_AT(void**, firstArray, 0x00) = nullptr;
    CSF_AT(uint32_t, firstArray, 0x04) = 0;
    CSF_AT(void**, secondArray, 0x00) = nullptr;
    CSF_AT(uint32_t, secondArray, 0x04) = 0;
    CSF_AT(uint32_t, secondArray, 0x08) = 0;
    CSF_AT(uint32_t, firstArray, 0x08) = 0;

    RpClumpForAllAtomics(
        firstClump,
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006BD100)),
        reinterpret_cast<int>(firstArray));

    CSF_AT(uint32_t, secondArray, 0x08) = 0;
    RpClumpForAllAtomics(
        secondClump,
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006BD100)),
        reinterpret_cast<int>(secondArray));

    uint32_t firstIndex = 0;
    do
    {
        if (firstIndex >= CSF_AT(uint32_t, firstArray, 0x08))
            break;

        const uint32_t resource = CSF_AT(
            uint32_t,
            CSF_AT(void**, firstArray, 0x00)[firstIndex],
            0x18);

        uint32_t secondIndex = 0;
        while (!matched)
        {
            if (secondIndex >= CSF_AT(uint32_t, secondArray, 0x08))
                break;

            if (CSF_AT(uint32_t,
                       CSF_AT(void**, secondArray, 0x00)[secondIndex],
                       0x18) == resource)
            {
                matched = 1;
            }
            ++secondIndex;
        }

        ++firstIndex;
    } while (!matched);

    if (CSF_AT(void**, secondArray, 0x00))
    {
        CoreMemory_Free(CSF_AT(void**, secondArray, 0x00));
        CSF_AT(void**, secondArray, 0x00) = nullptr;
    }

    if (CSF_AT(void**, firstArray, 0x00))
        CoreMemory_Free(CSF_AT(void**, firstArray, 0x00));

    return matched;
}

HOOK(0x006C0EE0, GfxTexDict_ClumpsShareResource, "auto");
