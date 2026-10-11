#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* edx, int value));

static int __fastcall GfxSkin_ApplySceneRegionToObject(C_0085e7b8** self, void* /*edx*/, int a2)
{
    // Object held at a2+0x1C; its vtable slot 7 (offset 0x1C) produces the target field owner.
    void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a2), 0x1C));

    // Scene region lookup uses the pointer stored at self+4 as `this`, with argument 7.
    int sceneRegion = GetSceneRegionAt(reinterpret_cast<void*>(self[1]), nullptr, 7);

    void* target = ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[7])(obj, nullptr, sceneRegion);

    return C_008670f0_SetField0C(reinterpret_cast<C_008670f0*>(target), nullptr, 0);
}

HOOK(0x006C58D0, GfxSkin_ApplySceneRegionToObject, "auto");
