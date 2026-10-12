#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpCollisionWorldForAllIntersections, 0x0056BAF0, (int world, void* box, uintptr_t callback, void* userData));

using IntersectionCallback = int (__cdecl *)(int, int, float*, uint32_t, int);
CSF_FUNC(int, __cdecl, GfxCollision_QueryWorldIntersections, 0x006EBA90,
         (int world, int box, IntersectionCallback callback, int userData));

static void __fastcall C_00862028_QueryIntersections(
    C_00862028* self, void* /*edx*/, int world, int userData)
{
    void* worldObject = reinterpret_cast<void*>(world);
    void* query = reinterpret_cast<void*>(userData);

    if ((CSF_AT(uint8_t, worldObject, 0xB) & 1) != 0)
    {
        reinterpret_cast<void (__fastcall *)(C_00862028*, void*, int, int)>(
            CSF_AT(void**, self, 0)[0x44 / 4])(self, nullptr, world, userData);
        return;
    }

    void* intersection = reinterpret_cast<void*>(CSF_AT(uint32_t, query, 0xC));
    const uint32_t intersectionType = CSF_AT(uint32_t, intersection, 0x34);
    float* box = reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(intersection) + 0x1C);

    if (intersectionType != 1)
    {
        RpCollisionWorldForAllIntersections(
            world, box, reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x006ED9A0)), query);
        return;
    }

    if (box[0] != box[3] || box[1] != box[4] || box[2] != box[5])
    {
        if (CSF_AT(uint32_t, query, 0x4) == 0xFFFFFF)
        {
            GfxCollision_QueryWorldIntersections(
                world, reinterpret_cast<int>(box),
                reinterpret_cast<IntersectionCallback>(CSF_ADDR_FN(0x006ED830)),
                userData);
            return;
        }

        RpCollisionWorldForAllIntersections(
            world, box, reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x006ED830)), query);
    }
}

HOOK(0x006EDED0, C_00862028_QueryIntersections, "auto");
