#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, MpSpawnPoints_IsSpawnChangeAllowed, 0x004F5740, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __fastcall, MpSpawnPoints_UpdateSpawnObject, 0x00554700, (void* self, void* edx, int arg0, int arg4, char a4));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __fastcall S_00552b80_UpdateSpawnPointState(
    S_00552b80* self, void* /*edx*/)
{
    auto* sceneObject = reinterpret_cast<C_00833b88*>(self->field_0c8c);
    if (!sceneObject)
    {
        MpSpawnPoints_UpdateSpawnObject(
            self, nullptr, static_cast<int>(self->field_13cc),
            static_cast<int>(self->field_13d0), 1);
    }
    else
    {
        const uint32_t embeddedObject = CSF_AT(uint32_t, sceneObject, 0x18);
        auto* owner = embeddedObject
            ? reinterpret_cast<uint8_t*>(embeddedObject) - 0x6B0
            : nullptr;

        if (static_cast<uint8_t>(SceneObj_IsInactive(sceneObject, nullptr)) ||
            !CSF_AT(uint8_t, owner, 0x443))
        {
            if (self->field_13cc !=
                    static_cast<uint32_t>(static_cast<int>(
                        CSF_AT(int8_t, owner, 0x458))) ||
                self->field_148c)
            {
                const int globalValue = GetGlobalValue_4FBDE0();
                const int firstElement = GetFirstElement(
                    reinterpret_cast<void*>(globalValue), nullptr);
                void* provider = GetCachedObjectProvider(
                    reinterpret_cast<void*>(firstElement), nullptr);

                if (MpSpawnPoints_IsSpawnChangeAllowed(provider, nullptr))
                {
                    MpSpawnPoints_UpdateSpawnObject(
                        self, nullptr, static_cast<int>(self->field_13cc),
                        static_cast<int>(self->field_13d0), 0);
                }
            }
        }
        else if (self->field_1401)
        {
            if (self->field_1402)
            {
                MpSpawnPoints_UpdateSpawnObject(
                    self, nullptr, static_cast<int>(self->field_13cc),
                    static_cast<int>(self->field_13d0), 0);
            }
            else if (!static_cast<uint8_t>(SceneObject_PassesFirstCheck(
                         reinterpret_cast<void*>(self->field_0c8c), nullptr)))
            {
                auto* currentSceneObject =
                    reinterpret_cast<C_00833b88*>(self->field_0c8c);
                void* controller =
                    reinterpret_cast<void*>(currentSceneObject->field_0038);
                reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, controller, 0)[0x9C / 4])(
                        controller, nullptr);
            }
        }
    }

    const int result = SceneObj_IsInactive(
        reinterpret_cast<void*>(self->field_0c8c), nullptr);
    if (static_cast<uint8_t>(result))
    {
        self->field_1401 = 0;
        self->field_1402 = 0;
    }
    return result;
}

HOOK(0x00554E30, S_00552b80_UpdateSpawnPointState, "auto");
