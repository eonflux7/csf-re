#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008316d8_ReleaseFirearmResources, 0x00476260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008316d8_ReleaseWeaponResources, 0x00475AD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall C_008316d8_Destroy(C_008316d8* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008316D8);
    C_008316d8_ReleaseFirearmResources(self, nullptr);
    C_008316d8_ReleaseWeaponResources(self, nullptr);

    int result = static_cast<int>(self->field_0008);
    if (result)
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(global, nullptr));
        result = reinterpret_cast<int (__fastcall*)(void*, void*, C_00833908*)>(
            CSF_AT(void**, controller, 0)[0x18 / 4])(
                controller, nullptr,
                reinterpret_cast<C_00833908*>(self->field_0008));
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x004764C0, C_008316d8_Destroy, "auto");
