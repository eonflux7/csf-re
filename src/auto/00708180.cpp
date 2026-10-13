#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(int, __cdecl, RpHAnimHierarchyUpdateMatrices, 0x0056D3F0, (int* a1));
CSF_FUNC(int, __cdecl, RpHAnimHierarchyAttach, 0x0056DB50, (int a1));
CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentAnim, 0x0061F050, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetHAnimChildFrameCount, 0x006E8090, (int a1));

static char __fastcall GfxAnimInterpolator_InitializeHierarchy(
    void* self, void* /*edx*/, int clump, int animation,
    int /*unused*/, char attachHierarchy)
{
    if (!clump)
        return 0;

    const int hierarchy = GetHAnimChildFrameCount(clump);
    CSF_AT(int, self, 0x04) = hierarchy;
    if (!hierarchy)
        return 0;

    RpClumpForAllAtomics(
        clump,
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006E80C0)),
        hierarchy);

    if (attachHierarchy)
        RpHAnimHierarchyAttach(CSF_AT(int, self, 0x04));

    CSF_AT(uint32_t, reinterpret_cast<void*>(CSF_AT(int, self, 0x04)), 0x00)
        |= 0x1000u;

    CSF_AT(int, self, 0x08) = animation;
    if (animation)
    {
        RtAnimInterpolatorSetCurrentAnim(
            CSF_AT(uint32_t*,
                   reinterpret_cast<void*>(CSF_AT(int, self, 0x04)), 0x20),
            reinterpret_cast<uint32_t*>(animation));
        RpHAnimHierarchyUpdateMatrices(
            reinterpret_cast<int*>(CSF_AT(int, self, 0x04)));
    }

    void* entries = Mem_Alloc(
        4 * CSF_AT(uint32_t,
                   reinterpret_cast<void*>(CSF_AT(int, self, 0x04)), 0x04));
    void* currentHierarchy =
        reinterpret_cast<void*>(CSF_AT(int, self, 0x04));
    CSF_AT(void*, self, 0x0C) = entries;

    int index = 0;
    if (CSF_AT(int, currentHierarchy, 0x04) > 0)
    {
        do
        {
            CSF_AT(uint32_t*, self, 0x0C)[index] = 0;
            ++index;
        } while (index < CSF_AT(
                     int, reinterpret_cast<void*>(CSF_AT(int, self, 0x04)),
                     0x04));
    }

    CSF_AT(float, self, 0x10) = 1.0f;
    return 1;
}

HOOK(0x00708180, GfxAnimInterpolator_InitializeHierarchy, "auto");
