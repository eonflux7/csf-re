#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpCollisionWorldForAllIntersections, 0x0056BAF0, (int world, void* box, uintptr_t callback, void* userData));

using IntersectionCallback = int (__cdecl *)(int, int, float*, uint32_t, int);

CSF_FUNC(int, __cdecl, GfxCollision_ForAllSpecialIntersections, 0x006EBA90,
         (int world, int intersection, IntersectionCallback callback, int userData));

static char __fastcall GfxCollision_HasIntersection(
    S_006ebe90* self, void* /*edx*/, int world, int intersection)
{
    char found = 0;
    if ((CSF_AT(uint8_t, reinterpret_cast<void*>(world), 0xB) & 1) != 0)
    {
        return static_cast<char>(
            reinterpret_cast<int (__fastcall *)(S_006ebe90*, void*, int, int)>(
                CSF_AT(void**, self, 0)[0x4C / 4])(
                self, nullptr, world, intersection));
    }

    if (CSF_AT(uint32_t, reinterpret_cast<void*>(intersection), 0x18) == 1)
    {
        GfxCollision_ForAllSpecialIntersections(
            world, intersection,
            reinterpret_cast<IntersectionCallback>(CSF_ADDR_FN(0x006EBA20)),
            reinterpret_cast<int>(&found));
    }
    else
    {
        RpCollisionWorldForAllIntersections(
            world, reinterpret_cast<void*>(intersection),
            reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x006EBA20)), &found);
    }
    return found;
}

HOOK(0x006EBE90, GfxCollision_HasIntersection, "auto");
