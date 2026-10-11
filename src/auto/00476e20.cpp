#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008316d8_StopEffects, 0x00476260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008316d8_ReleaseResources, 0x00475AD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int* __fastcall C_008316d8_Destroy(C_008316d8* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x008316D8);
    C_008316d8_StopEffects(self, nullptr);
    C_008316d8_ReleaseResources(self, nullptr);

    if (self->field_0008)
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(global, nullptr));
        reinterpret_cast<void (__fastcall*)(void*, void*, C_00833908*)>(
            CSF_AT(void**, controller, 0)[0x18 / 4])(
                controller, nullptr,
                reinterpret_cast<C_00833908*>(self->field_0008));
    }

    self->vtbl = CSF_VTBL(0x0082D410);
    if (flags & 1)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x00476E20, C_008316d8_Destroy, "auto");
