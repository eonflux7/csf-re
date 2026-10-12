#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetBit2OfField4, 0x006B8170, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));

using IntersectionCallback = int (__cdecl*)(int, float*, uint32_t, int);
CSF_FUNC(int, __cdecl, RpAtomicForAllIntersections, 0x0056BDF0,
         (int atomic, uint32_t* intersection, IntersectionCallback callback, int data));

static int __cdecl GfxCollision_FilterIntersectingAtomic(int atomic, int context)
{
    int attachment = GfxObjectProps_GetObjectAttachment(atomic);
    if (static_cast<uint8_t>(
            GetBit2OfField4(reinterpret_cast<void*>(attachment), nullptr)) == 0)
    {
        RpAtomicForAllIntersections(
            atomic,
            CSF_AT(uint32_t*, reinterpret_cast<void*>(context), 0x4),
            reinterpret_cast<IntersectionCallback>(CSF_ADDR_FN(0x006EBA30)),
            context + 1);
    }

    return CSF_AT(uint8_t, reinterpret_cast<void*>(context), 0x1) != 0
               ? 0
               : atomic;
}

HOOK(0x006EBA40, GfxCollision_FilterIntersectingAtomic, "auto");
