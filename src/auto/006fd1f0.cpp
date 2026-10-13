#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, GfxLights_RemovePointerFromList, 0x006D16D0, (void* self, void* , int target));

// Returns the sub-object reached through the object's slot-9 accessor (vtable offset 0x24).
static void* __fastcall CLightObject_GetSubObject(CLightObject* self)
{
    return ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr);
}

static void __fastcall CLightObject_PrepareSubObjectForDispatch(CLightObject* self, void* /*edx*/)
{
    void* subObject = CLightObject_GetSubObject(self);
    bool isReady = ((bool (__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[0x70 / 4])(subObject, nullptr);
    if (isReady)
    {
        void* subObjectAgain = CLightObject_GetSubObject(self);
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, subObjectAgain, 0)[0x68 / 4])(subObjectAgain, nullptr, 0);
        GfxLights_RemovePointerFromList(CSF_AT(void*, self, 0x08), nullptr,
                                        static_cast<int>(reinterpret_cast<uintptr_t>(self)));
    }
}

HOOK(0x006FD1F0, CLightObject_PrepareSubObjectForDispatch, "auto");
