#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpWorldStreamGetSize, 0x0059FCB0, (uint32_t* a1));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveStreamSlotValue, 0x006BC270, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetMemberAtA8, 0x006BC2A0, (void* self, void* , int));

static int __fastcall C_00860bb8_CacheStreamSizes(C_00860bb8* self, void* /*edx*/)
{
    // Copy field +0x1C into +0xE4 before resolving the stream slot.
    uint32_t sourceValue = self->field_001c;
    uint32_t slotId = self->field_0008;
    self->field_00e4 = sourceValue;

    self->field_00d8 = ResolveStreamSlotValue(reinterpret_cast<void*>(static_cast<uintptr_t>(slotId)), nullptr);

    // Re-read +0x08 for the second call.
    int memberAtA8 = GetMemberAtA8(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr, 0);
    self->field_00dc = static_cast<uint32_t>(memberAtA8);

    // Stream size of the referenced object.
    void* referencedObject = GetReferencedObject(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr);
    if (referencedObject)
        self->field_00ec = static_cast<uint32_t>(RpWorldStreamGetSize(reinterpret_cast<uint32_t*>(referencedObject)));
    else
        self->field_00ec = 0;

    // Stream size of the world object collection.
    int worldObjectCollection = SceneLoader_GetWorldObjectCollection(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_000c)), nullptr);
    if (worldObjectCollection)
        self->field_00f0 = static_cast<uint32_t>(RpWorldStreamGetSize(reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(worldObjectCollection))));
    else
        self->field_00f0 = 0;

    return static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_00d8));
}

HOOK(0x006CB610, C_00860bb8_CacheStreamSizes, "auto");
