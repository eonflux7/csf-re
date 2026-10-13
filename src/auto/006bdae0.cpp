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

// Callees of SceneLoader_ClearRenderResources, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __fastcall, C_008605d8_RegisterPreparedScene, 0x006ECBE0,
         (C_008605d8* self, void* /*edx*/, C_00833708* scene, char enabled));

// Releases the prepared render resources held by the scene loader and clears
// the associated handles. Returns the result of the free call, or 0 if there was none.
static int __fastcall SceneLoader_ClearRenderResources(C_008605d8* self, void* /*edx*/)
{
    self->field_0091 = 0;

    // Register this loader's prepared scene with the scene controller.
    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    int sceneController = SceneLoader_GetSceneController(globalObject, nullptr);

    // Slot +0x38 of the controller's vtable returns the render target object.
    int renderTargetValue = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, reinterpret_cast<void*>(sceneController), 0)[56 / 4])(
        reinterpret_cast<void*>(sceneController), nullptr);
    C_008605d8* renderTarget = reinterpret_cast<C_008605d8*>(renderTargetValue);

    C_008605d8_RegisterPreparedScene(renderTarget, nullptr,
                                     reinterpret_cast<C_00833708*>(self), 0);

    // Ask the clump manager to release the resource identified by field_0004.
    void* globalClumpHost = reinterpret_cast<void*>(GetGlobalInt());
    int clumpManager = ResolveClumpManager(reinterpret_cast<int>(globalClumpHost), nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, reinterpret_cast<void*>(clumpManager), 0)[132 / 4])(
        reinterpret_cast<void*>(clumpManager), nullptr, self->field_0004);

    // Free the owned block at field_0184 if present, then clear the handles.
    int result = static_cast<int>(self->field_0184);
    if (result != 0)
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0184));

    self->field_0184 = 0;
    self->field_0188 = 0;
    self->field_018c = 0;
    return result;
}

HOOK(0x006BDAE0, SceneLoader_ClearRenderResources, "auto");
