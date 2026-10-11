#include <cstdint>
#include "globals.h"
#include "hook.h"

// RpHAnimHierarchyCreateFromHierarchy(hierarchy, flags, sourceData)
CSF_FUNC(uint32_t*, __cdecl, RpHAnimHierarchyCreateFromHierarchy, 0x0056CED0, (int a1, int a2, int a3));

// Creates a HAnim hierarchy from the hierarchy stored in the source object's
// sub-object at +0x20. The flags word of `self` is passed on with bit 0 cleared
// and bit 1 set.
static int __cdecl GfxHAnim_CreateHierarchyFromSourceObject(uint32_t* self)
{
    uint32_t flags = CSF_AT(uint32_t, self, 0x0);
    uint32_t sourceSub = CSF_AT(uint32_t, self, 0x20);
    uint32_t sourceData = CSF_AT(uint32_t, sourceSub, 0x20);

    uint32_t newFlags = (flags & 0xFFFFFFFEu) | 2u;

    uint32_t* created = RpHAnimHierarchyCreateFromHierarchy(
        static_cast<int>(reinterpret_cast<uintptr_t>(self)),
        static_cast<int>(newFlags),
        static_cast<int>(sourceData));

    return static_cast<int>(reinterpret_cast<uintptr_t>(created));
}

HOOK(0x006E80F0, GfxHAnim_CreateHierarchyFromSourceObject, "auto");
