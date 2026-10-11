#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, WeaponFirearms_ClearState, 0x00478C10,
                (void* self, void* edx));
static CSF_FUNC(int, __fastcall, WeaponFirearms_DestroyBase, 0x004764C0,
                (void* self, void* edx));
static CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010,
                (void* self, void* edx));
static CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall C_008318c8_Destroy(C_008318c8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008318C8);
    WeaponFirearms_ClearState(self, nullptr);

    void* ownedObject = reinterpret_cast<void*>(self->field_0088);
    if (ownedObject)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_0088 = 0;
    }

    if (self->field_0078)
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* sceneController = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(global, nullptr));
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, sceneController, 0)[6])(
                sceneController, nullptr, self->field_0078);
    }

    self->vtbl = CSF_VTBL(0x008317C8);
    WeaponFirearms_ClearState(self, nullptr);
    CSF_AT(void*, self, 0x64) = CSF_VTBL(0x0082D410);
    return WeaponFirearms_DestroyBase(self, nullptr);
}

HOOK(0x00479BB0, C_008318c8_Destroy, "auto");
