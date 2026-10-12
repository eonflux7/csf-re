#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetBit2OfField4, 0x006B8170, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __cdecl, RpAtomicForAllIntersections, 0x0056BDF0,
    (int atomic, uint32_t* intersection,
     int (__cdecl* callback)(int, float*, uint32_t, int), int context));

static int __cdecl GfxCollision_QueryAtomicIntersections(int atomic, int query)
{
    void* attachment = reinterpret_cast<void*>(
        GfxObjectProps_GetObjectAttachment(atomic));

    if (static_cast<uint8_t>(GetBit2OfField4(attachment, nullptr)) == 0)
    {
        alignas(8) uint8_t context[0x10];
        CSF_AT(int, context, 0x8) = CSF_AT(int, query, 0x4);
        CSF_AT(uint8_t, context, 0x4) = CSF_AT(uint8_t, query, 0x1);
        CSF_AT(uint8_t, context, 0x5) = CSF_AT(uint8_t, query, 0x3);
        uint32_t* intersection = CSF_AT(uint32_t*, query, 0x8);
        CSF_AT(int, context, 0x0) = atomic;
        CSF_AT(uint8_t, context, 0xC) = 0;

        RpAtomicForAllIntersections(
            atomic, intersection,
            reinterpret_cast<int (__cdecl*)(int, float*, uint32_t, int)>(
                CSF_ADDR_FN(0x006EC510)),
            reinterpret_cast<int>(context));
    }

    return CSF_AT(uint8_t, query, 0x2) != 0 ? 0 : atomic;
}

HOOK(0x006EC580, GfxCollision_QueryAtomicIntersections, "auto");
