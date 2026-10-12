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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));

static int __fastcall C_0082ddf8_ReleaseResourcesAndReset(C_0082ddf8* self, void* /*edx*/)
{
    // Stop the scene-side object that is referenced by field_0024 (if any).
    if (self->field_0024)
    {
        void* globalInt = reinterpret_cast<void*>(GetGlobalInt());
        void* sceneController = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(globalInt, nullptr));

        // Virtual slot 33 (+0x84) on the scene controller.
        auto getSubObjectFn = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneController, 0)[33]);
        void* subObject = reinterpret_cast<void*>(getSubObjectFn(sceneController, nullptr));

        // Virtual slot 1 (+0x04) on the sub-object, passing &field_0024.
        auto releaseFn = reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, subObject, 0)[1]);
        releaseFn(subObject, nullptr, &self->field_0024);
    }

    // Free the owned block at field_0020 (matrix destroy first if flagged).
    S_006adb40* block = reinterpret_cast<S_006adb40*>(self->field_0020);
    self->field_0010 = 0;
    if (block)
    {
        if (CSF_AT(uint8_t, block, 0x4))
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(CSF_AT(uint32_t, block, 0x0)));
        Mem_Free(block);
        self->field_0020 = 0;
    }

    // Release the object at field_001c via its vtable slot 0 with arg 1.
    void* ownedObject = reinterpret_cast<void*>(self->field_001c);
    if (ownedObject)
    {
        auto destroyFn = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0]);
        destroyFn(ownedObject, nullptr, 1);
        self->field_001c = 0;
    }

    self->field_0018 = 0;
    self->field_0019 = 0;
    self->field_001a = 0;

    // Tail jump into the base class release (forwards ecx=this, no stack args).
    return C_0082deb8_ReleaseChildBase_41E110(reinterpret_cast<C_0082e520*>(self), nullptr);
}

HOOK(0x0040F0D0, C_0082ddf8_ReleaseResourcesAndReset, "auto");
