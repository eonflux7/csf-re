#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __cdecl, RwFrameCreate, 0x006235D0, ());
CSF_FUNC(uint32_t*, __cdecl, GetShapeAllocatorHandle, 0x006EEA50, ());
CSF_FUNC(uint32_t*, __cdecl, GetShapeRegistryHandle, 0x006D4000, ());
CSF_FUNC(uint32_t*, __fastcall, ConstructSphereShapeList, 0x006C9780, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, ConstructEllipsoidShapeList, 0x006CECD0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, ConstructShapeBoundsCache, 0x006CFBE0, (void* self, void* edx));

// Sets up the collision-shape sub-objects of a gfx collision shape table:
// allocator handle, sphere list, registry handle, ellipsoid list,
// bounds cache, and the frame object used for shape transforms.
static bool __fastcall InitGfxCollisionShapeTables(uint8_t* self, void* /*edx*/)
{
    CSF_AT(uint32_t*, self, 0x04) = GetShapeAllocatorHandle();

    void* sphereBlock = Mem_Alloc(20);
    if (sphereBlock)
        CSF_AT(uint32_t*, self, 0x08) = ConstructSphereShapeList(sphereBlock, nullptr);
    else
        CSF_AT(uint32_t*, self, 0x08) = nullptr;

    CSF_AT(uint32_t*, self, 0x0C) = GetShapeRegistryHandle();

    void* ellipsoidBlock = Mem_Alloc(24);
    if (ellipsoidBlock)
        CSF_AT(uint32_t*, self, 0x10) = ConstructEllipsoidShapeList(ellipsoidBlock, nullptr);
    else
        CSF_AT(uint32_t*, self, 0x10) = nullptr;

    void* boundsBlock = Mem_Alloc(20);
    if (boundsBlock)
        CSF_AT(uint32_t*, self, 0x14) = ConstructShapeBoundsCache(boundsBlock, nullptr);
    else
        CSF_AT(uint32_t*, self, 0x14) = nullptr;

    CSF_AT(void*, self, 0x6C) = RwFrameCreate();
    return true;
}

HOOK(0x006CF710, InitGfxCollisionShapeTables, "auto");
