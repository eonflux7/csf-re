#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, GfxCollision_AttachmentMatchesFilter, 0x006B8100, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));

static int __cdecl GfxCollision_CollectMatchingObject(int objectIndex, void* context)
{
    int attachment = GfxObjectProps_GetObjectAttachment(objectIndex);
    if (static_cast<uint8_t>(GfxCollision_AttachmentMatchesFilter(
            reinterpret_cast<void*>(attachment), nullptr,
            CSF_AT(int, context, 0x64))))
    {
        if (CSF_AT(uint32_t, context, 0x5C) >=
            CSF_AT(uint32_t, context, 0x58))
        {
            (void)CSF_VTBL(0x0041E5F0);
            S_0041e5f0_GrowArrayCapacity(
                reinterpret_cast<S_0041e5f0*>(
                    static_cast<uint8_t*>(context) + 0x54),
                nullptr, CSF_AT(int, context, 0x60));
        }

        uint32_t count = CSF_AT(uint32_t, context, 0x5C);
        int* destination = CSF_AT(int*, context, 0x54) + count;
        CSF_AT(uint32_t, context, 0x5C) = count + 1;
        *destination = objectIndex;
    }
    return objectIndex;
}

HOOK(0x006ECA40, GfxCollision_CollectMatchingObject, "auto");
