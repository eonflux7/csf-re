#include <cstdint>
#include "globals.h"
#include "hook.h"

// Resets the color override on the object attached at +0x14.
// Flag bit 4 at +0x40 selects which color (0x3f000000 = 0.5 or 0x3f800000 = 1.0) is set on the
// attached object's sub-object before the final call.
static int __fastcall SceneObject_ResetAttachedColorOverride(void* self, void* /*edx*/)
{
    void* attachedObject = CSF_AT(void*, self, 0x14);
    void* colorObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, attachedObject, 0)[10])(attachedObject, nullptr);

    if ((CSF_AT(uint8_t, self, 0x40) & 4) != 0)
    {
        // Color vector 0x3e24a4a5, 0x3f7afafb, 0x3dc8c8c9, 0x3f800000 (raw dwords, as written by the original).
        uint32_t colorWords[4] = { 0x3e24a4a5u, 0x3f7afafbu, 0x3dc8c8c9u, 0x3f800000u };
        ((void (__fastcall*)(void*, void*, uint32_t*, int))CSF_AT(void**, colorObject, 0)[23])(colorObject, nullptr, colorWords, 0);

        void* subObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, colorObject, 0)[20])(colorObject, nullptr);
        return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, subObject, 0)[1])(subObject, nullptr, 0x3f000000);
    }
    else
    {
        ((void (__fastcall*)(void*, void*))CSF_AT(void**, colorObject, 0)[22])(colorObject, nullptr);

        void* subObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, colorObject, 0)[20])(colorObject, nullptr);
        return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, subObject, 0)[1])(subObject, nullptr, 0x3f800000);
    }
}

HOOK(0x4CF850, SceneObject_ResetAttachedColorOverride, "auto");
