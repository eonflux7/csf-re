#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpCollisionWorldForAllIntersections, 0x0056BAF0, (int world, void* box, uintptr_t callback, void* userData));

using IntersectionCallback = int (__cdecl *)(int, int, float*, uint32_t, int);
CSF_FUNC(int, __cdecl, GfxCollision_QuerySpecialIntersections, 0x006EBA90,
         (int world, int intersection, IntersectionCallback callback, int userData));

static char __fastcall GfxCollision_QueryWorldIntersections(
    void* self, void* /*edx*/, int world, int queryData)
{
    void* worldObject = reinterpret_cast<void*>(world);
    void* query = reinterpret_cast<void*>(queryData);

    if ((CSF_AT(uint8_t, worldObject, 0xB) & 1) != 0)
    {
        reinterpret_cast<void (__fastcall *)(void*, void*, int, int)>(
            CSF_AT(void**, self, 0)[0x54 / 4])(
                self, nullptr, world, queryData);
    }
    else
    {
        int intersection = CSF_AT(int, query, 0x8);
        if (CSF_AT(int, reinterpret_cast<void*>(intersection), 0x18) == 1 &&
            CSF_AT(int, query, 0x4) == 0xFFFFFF)
        {
            GfxCollision_QuerySpecialIntersections(
                world, intersection,
                reinterpret_cast<IntersectionCallback>(CSF_ADDR_FN(0x006EC450)),
                queryData);
        }
        else
        {
            RpCollisionWorldForAllIntersections(
                world, reinterpret_cast<void*>(intersection),
                reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x006EC450)), query);
        }
    }

    return CSF_AT(char, query, 0x2);
}

HOOK(0x006ECD80, GfxCollision_QueryWorldIntersections, "auto");
