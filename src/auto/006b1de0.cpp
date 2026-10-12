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

// The original references S_0041e5f0's vtable at 0x6B1E31 (the call site of GrowArrayCapacity).
[[maybe_unused]] static auto s_SceneLoaderArrayVtbl = CSF_VTBL(0x0041E5F0);

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, SkinRegion_Construct, 0x006C4490, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureGeometry, 0x006C35C0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_008656d0_SetSkinRegionFromTarget, 0x006C58A0, (C_008656d0* self, void* , C_008656d0* targetArg));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// __thiscall method written as __fastcall free function; ret 8 => two stack args.
static C_008656d0* __fastcall SceneLoader_CreateIndexedGeometry(C_00860130* self, void* /*edx*/, int a2, int a3)
{
    C_008656d0* createdObject = nullptr;

    C_00860848* skinRegion = reinterpret_cast<C_00860848*>(Mem_Alloc(52));
    if (skinRegion)
    {
        createdObject = reinterpret_cast<C_008656d0*>(
            static_cast<std::uintptr_t>(SkinRegion_Construct(skinRegion, nullptr)));
    }

    SceneLoader_ConfigureGeometry(createdObject, nullptr, a2, a3);

    C_008656d0* globalValue = reinterpret_cast<C_008656d0*>(
        static_cast<std::uintptr_t>(GetGlobalValue_6C5760()));
    C_008656d0_SetSkinRegionFromTarget(globalValue, nullptr, createdObject);

    uint32_t countField = self->field_0010;
    uint32_t capacityField = self->field_000c;
    S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(&self->field_0008);
    if (countField >= capacityField)
        S_0041e5f0_GrowArrayCapacity(list, nullptr, list->field_000c);

    uint32_t index = list->field_0008;
    C_008656d0** slot = reinterpret_cast<C_008656d0**>(list->field_0000 + 4 * index);
    list->field_0008 = index + 1;
    *slot = createdObject;

    return createdObject;
}

HOOK(0x006B1DE0, SceneLoader_CreateIndexedGeometry, "auto");
