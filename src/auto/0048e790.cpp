#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall SceneObject_SetColorOverride(void* self, void* /*edx*/, int color)
{
    void* renderObject = CSF_AT(void*, self, 0x16C);
    if (!renderObject)
        return 0;

    return reinterpret_cast<int (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, renderObject, 0)[0x5C / 4])(
        renderObject, nullptr, color, 1);
}

HOOK(0x0048E790, SceneObject_SetColorOverride, "auto");
