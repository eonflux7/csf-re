#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

static int __fastcall CommandoAnims_FinalizeGlobalResources(void* self, void* /*edx*/)
{
    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    reinterpret_cast<void(__fastcall*)(void*, void*, const char*, int)>(
        CSF_AT(void**, sceneController, 0)[4])(
            sceneController, nullptr, CSF_STR(0x0083F1BC, "Models/Char/Cabeza.dff"), 1);

    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* animationManager = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, accessor, 0)[5])(accessor, nullptr);

    CSF_AT(int, self, 0x38) = -1;
    CSF_AT(int, self, 0x3C) = -1;
    CSF_AT(int, self, 0x40) = -1;
    CSF_AT(int, self, 0x44) = -1;
    CSF_AT(int, self, 0x48) = -1;
    CSF_AT(int, self, 0x4C) = -1;
    CSF_AT(int, self, 0x50) = -1;
    CSF_AT(int, self, 0x54) = -1;

    alignas(8) uint8_t options[0x4] = {};
    using LoadAnimationFn = int(__fastcall*)(void*, void*, const char*, uint8_t*);

    CSF_AT(int, self, 0x3C) = reinterpret_cast<LoadAnimationFn>(
        CSF_AT(void**, animationManager, 0)[4])(
            animationManager, nullptr, CSF_STR(0x0083F430, "Anims/Comm/Correr.anm"), options);

    CSF_AT(int, self, 0x40) = reinterpret_cast<LoadAnimationFn>(
        CSF_AT(void**, animationManager, 0)[4])(
            animationManager, nullptr, CSF_STR(0x0083F418, "Anims/Comm/Andar.anm"), options);

    CSF_AT(int, self, 0x44) = reinterpret_cast<LoadAnimationFn>(
        CSF_AT(void**, animationManager, 0)[4])(
            animationManager, nullptr, CSF_STR(0x0083F400, "Anims/Comm/Gatear.anm"), options);

    CSF_AT(int, self, 0x50) = reinterpret_cast<LoadAnimationFn>(
        CSF_AT(void**, animationManager, 0)[4])(
            animationManager, nullptr, CSF_STR(0x0083F3E8, "Anims/Comm/Morir.anm"), options);

    CSF_AT(int, self, 0x4C) = reinterpret_cast<LoadAnimationFn>(
        CSF_AT(void**, animationManager, 0)[4])(
            animationManager, nullptr, CSF_STR(0x0083F3CC, "Anims/Comm/Agacharse.anm"), options);

    CSF_AT(int, self, 0x48) = reinterpret_cast<LoadAnimationFn>(
        CSF_AT(void**, animationManager, 0)[4])(
            animationManager, nullptr, CSF_STR(0x0083F3B0, "Anims/Comm/Levantarse.anm"), options);

    const int idleAnimation = reinterpret_cast<LoadAnimationFn>(
        CSF_AT(void**, animationManager, 0)[4])(
            animationManager, nullptr, CSF_STR(0x0083F39C, "Anims/Comm/Idle.anm"), options);
    CSF_AT(int, self, 0x54) = idleAnimation;
    return idleAnimation;
}

HOOK(0x004FBED0, CommandoAnims_FinalizeGlobalResources, "auto");
