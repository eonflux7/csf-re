#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyGetMatrixArray, 0x0056CF80, (int a1));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_008633a0_InitializeBone, 0x007075F0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));

// Returns the cached bone handle for boneIndex, creating it on first use.
static int __fastcall C_00863430_GetOrCreateBoneHandle(C_00863430* self, void* /*edx*/, int boneIndex)
{
    int* cache = reinterpret_cast<int*>(self->field_000c);
    if (cache[boneIndex] != 0)
        return cache[boneIndex];

    uint8_t* hierarchy = reinterpret_cast<uint8_t*>(self->field_0004);

    // Bone entry: 16 bytes per bone inside the table at hierarchy+0x10.
    int* boneEntry = reinterpret_cast<int*>(CSF_AT(uint32_t, hierarchy, 0x10) + 16 * boneIndex);

    // Bone data address: boneIndex * stride + sub-object base + 76.
    uint32_t subBase = CSF_AT(uint32_t, hierarchy, 0x20);
    uint8_t* subObject = reinterpret_cast<uint8_t*>(subBase);
    int boneDataAddr = boneIndex * CSF_AT(int, subObject, 0x24) + static_cast<int>(subBase) + 76;

    int matrixArray = RpHAnimHierarchyGetMatrixArray(static_cast<int>(self->field_0004));
    int boneMatrix = matrixArray + (boneIndex << 6);

    void* allocated = Mem_Alloc(36);
    int result;
    if (allocated)
        result = C_008633a0_InitializeBone(allocated, nullptr, *boneEntry, 0,
                                           boneDataAddr + 8, boneDataAddr + 24,
                                           boneMatrix, reinterpret_cast<int>(self));
    else
        result = 0;

    cache = reinterpret_cast<int*>(self->field_000c);
    cache[boneIndex] = result;
    return cache[boneIndex];
}

HOOK(0x007083A0, C_00863430_GetOrCreateBoneHandle, "auto");
