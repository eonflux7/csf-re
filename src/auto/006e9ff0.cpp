#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(int, __cdecl, RpHAnimHierarchyGetMatrixArray, 0x0056CF80, (int a1));
CSF_FUNC(int, __cdecl, RpHAnimHierarchyUpdateMatrices, 0x0056D3F0, (int* a1));
CSF_FUNC(int, __cdecl, RpHAnimHierarchyAttach, 0x0056DB50, (int a1));
CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentAnim, 0x0061F050, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GfxHanim_FindClumpHierarchy, 0x006E8090, (int a1));
CSF_FUNC(int, __cdecl, GfxHanim_FindHierarchyNode, 0x006E8120, (int a1, int a2));
CSF_FUNC(char, __fastcall, GfxHanim_GrowHierarchyEntries, 0x006E8EA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8BBB28, 0x006F7820, ());
CSF_FUNC(int, __fastcall, GfxHanim_RegisterHierarchy, 0x006F7960, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(int, g_SavedMatrixNodeIndices, 0x008A8090);

static char __fastcall GfxHanim_InitializeClump(
    void* self, void* /*edx*/, int clump, int animation,
    char saveMatrices, char attachHierarchy)
{
    CSF_AT(int, self, 0x1C) = clump;
    if (saveMatrices)
        CSF_AT(uint32_t, self, 0x58) |= 1u;

    if (!clump)
        return 0;

    const int hierarchy = GfxHanim_FindClumpHierarchy(clump);
    CSF_AT(int, self, 0x14) = hierarchy;
    if (!hierarchy)
    {
        CSF_AT(int, self, 0x1C) = 0;
        return 0;
    }

    RpClumpForAllAtomics(
        CSF_AT(int, self, 0x1C),
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006E80C0)),
        hierarchy);
    CSF_AT(uint32_t, self, 0x0C) = 0;

    if (attachHierarchy)
        RpHAnimHierarchyAttach(CSF_AT(int, self, 0x14));

    uint32_t entry[9] = {};
    CSF_AT(uint32_t, CSF_AT(void*, self, 0x14), 0) |= 0x1000;
    entry[0] = CSF_AT(uint32_t, self, 0x14);
    entry[5] = 544;

    if (CSF_AT(uint32_t, self, 0x0C) >= CSF_AT(uint32_t, self, 0x08))
    {
        GfxHanim_GrowHierarchyEntries(
            static_cast<uint8_t*>(self) + 4, nullptr,
            CSF_AT(int, self, 0x10));
    }

    const uint32_t entryIndex = CSF_AT(uint32_t, self, 0x0C);
    void* entries = CSF_AT(void*, self, 0x04);
    CSF_AT(uint32_t, self, 0x0C) = entryIndex + 1;
    std::memcpy(static_cast<uint8_t*>(entries) + 36 * entryIndex,
                entry, 0x24);

    uint32_t* hierarchyToRegister = CSF_AT(uint32_t*, self, 0x14);
    CSF_AT(int, self, 0x20) = animation;
    void* registry = reinterpret_cast<void*>(GetValue8BBB28());
    CSF_AT(int, self, 0x18) =
        GfxHanim_RegisterHierarchy(registry, nullptr, hierarchyToRegister);

    if (CSF_AT(int, self, 0x20))
    {
        RtAnimInterpolatorSetCurrentAnim(
            CSF_AT(uint32_t*, CSF_AT(void*, self, 0x14), 0x20),
            CSF_AT(uint32_t*, self, 0x20));
        RpHAnimHierarchyUpdateMatrices(CSF_AT(int*, self, 0x14));
    }

    if (saveMatrices)
    {
        CSF_AT(int, self, 0x30) =
            GfxHanim_FindHierarchyNode(CSF_AT(int, self, 0x14), 1);
    }
    CSF_AT(int, self, 0x34) =
        GfxHanim_FindHierarchyNode(CSF_AT(int, self, 0x14), 10);

    CSF_AT(void*, self, 0x4C) = Mem_Alloc(
        4u * CSF_AT(uint32_t, CSF_AT(void*, self, 0x14), 0x04));

    for (int node = 0;
         node < CSF_AT(int, CSF_AT(void*, self, 0x14), 0x04);
         ++node)
    {
        CSF_AT(uint32_t*, self, 0x4C)[node] = 0;
    }

    if (saveMatrices && g_SavedMatrixNodeIndices >= 0)
    {
        CSF_AT(void*, self, 0x50) = Mem_Alloc(
            4u * CSF_AT(uint32_t, CSF_AT(void*, self, 0x14), 0x04));

        for (int node = 0;
             node < CSF_AT(int, CSF_AT(void*, self, 0x14), 0x04);
             ++node)
        {
            CSF_AT(uint32_t*, self, 0x50)[node] = 0;
        }

        if (g_SavedMatrixNodeIndices >= 0)
        {
            int* nodeIndex = &g_SavedMatrixNodeIndices;
            do
            {
                const int node = *nodeIndex;
                if (node < CSF_AT(int, CSF_AT(void*, self, 0x14), 0x04))
                {
                    uint32_t* matrix = RwMatrixCreate();
                    CSF_AT(uint32_t**, self, 0x50)[node] = matrix;

                    const int matrixArray = RpHAnimHierarchyGetMatrixArray(
                        CSF_AT(int, self, 0x14));
                    const uint32_t sourceAddress =
                        static_cast<uint32_t>(matrixArray) +
                        (static_cast<uint32_t>(node) << 6);
                    std::memcpy(
                        CSF_AT(void**, self, 0x50)[node],
                        reinterpret_cast<const void*>(sourceAddress), 0x40);
                }
                ++nodeIndex;
            } while (*nodeIndex >= 0);
        }
    }

    CSF_AT(float, self, 0x54) = 1.0f;
    return 1;
}

HOOK(0x006E9FF0, GfxHanim_InitializeClump, "auto");
