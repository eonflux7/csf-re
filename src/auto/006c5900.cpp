#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void*, int value));
CSF_FUNC(int, __fastcall, Subobject_SetKeyedValue, 0x006C4BF0, (void* self, void* edx, int a2, unsigned int a3));

// Virtual slot 0x1c on the value-pointer object returns a sub-object (vtable slot 7).
static void* GetValuePointerSubobject(void* valuePointer)
{
    return ((void* (__fastcall*)(void*, void*))CSF_AT(void**, valuePointer, 0)[0x1c / 4])(valuePointer, nullptr);
}

static int __fastcall GfxSkin_SetHandleSceneRegion(C_0085e7b8** self, void* /*edx*/, C_00862490* a2)
{
    int valuePointer = Handle_GetValuePointer(a2, nullptr);
    int sceneRegion = GetSceneRegionAt(CSF_AT(void*, self, 4), nullptr, 4);

    C_008670f0* regionField = reinterpret_cast<C_008670f0*>(GetValuePointerSubobject(reinterpret_cast<void*>(valuePointer)));
    C_008670f0_SetField0C(regionField, nullptr, sceneRegion);

    void* keyedObjA = GetValuePointerSubobject(reinterpret_cast<void*>(valuePointer));
    Subobject_SetKeyedValue(keyedObjA, nullptr, 6, 0);

    void* keyedObjB = GetValuePointerSubobject(reinterpret_cast<void*>(valuePointer));
    return Subobject_SetKeyedValue(keyedObjB, nullptr, 8, 0);
}

HOOK(0x006C5900, GfxSkin_SetHandleSceneRegion, "auto");
