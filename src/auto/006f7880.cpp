#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GfxHAnim_CreateHierarchyFromSourceObject, 0x006E80F0, (uint32_t* self));

static int* __fastcall GfxMaterials_CreateHAnimInstance(
    uint32_t* self, void* /*edx*/, uint32_t* sourceObject,
    int typeIndex, int variant)
{
    if (typeIndex < 0 || typeIndex >= CSF_AT(int, self, 0x8))
        return nullptr;

    void* entry = CSF_AT(void**, self, 0x0)[typeIndex];
    void* pools = reinterpret_cast<uint8_t*>(entry) + 0xC;
    int* instance = nullptr;

    if (CSF_AT(uint32_t, entry, 0x14) != 0)
    {
        uint32_t poolIndex = 0;
        do
        {
            void* pool = CSF_AT(void**, pools, 0x0)[poolIndex];
            if (CSF_AT(int, pool, 0x10) == variant &&
                CSF_AT(uint32_t, pool, 0x8) != 0)
            {
                uint32_t count = CSF_AT(uint32_t, pool, 0x8);
                instance = CSF_AT(int**, pool, 0x0)[count - 1];
                CSF_AT(uint32_t, pool, 0x8) = count - 1;
            }

            ++poolIndex;
            if (poolIndex >= CSF_AT(uint32_t, pools, 0x8))
                break;
        } while (!instance);
    }

    if (instance)
    {
        CSF_AT(uint32_t*, CSF_AT(void*, instance, 0x8), 0x18) = sourceObject;
        CSF_AT(uint32_t*, CSF_AT(void*, instance, 0xC), 0x18) = sourceObject;
        return instance;
    }

    instance = reinterpret_cast<int*>(CoreMemory_AllocUntagged(24));
    if (instance)
    {
        CSF_AT(int, instance, 0x8) = 0;
        CSF_AT(int, instance, 0xC) = 0;
        CSF_AT(int, instance, 0x10) = 0;
        CSF_AT(int, instance, 0x14) = 0;
        CSF_AT(int, instance, 0x0) = -1;
        CSF_AT(int, instance, 0x4) = -1;
    }

    CSF_AT(int, instance, 0x0) = typeIndex;
    CSF_AT(int, instance, 0x4) = variant;
    CSF_AT(int, instance, 0x8) =
        GfxHAnim_CreateHierarchyFromSourceObject(sourceObject);
    CSF_AT(int, instance, 0xC) =
        GfxHAnim_CreateHierarchyFromSourceObject(sourceObject);
    return instance;
}

HOOK(0x006F7880, GfxMaterials_CreateHAnimInstance, "auto");
