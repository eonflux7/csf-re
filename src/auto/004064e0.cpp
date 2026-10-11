#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(const char*, g_psz_PFX_IMPACTO_BALA, 0x008A8BD0);

static int __fastcall C_0082d70c_CreateImpactEffect(
    C_0082d70c* self, void* /*edx*/, int attachmentId, int effectType, char resetEffect)
{
    auto* sceneObject = reinterpret_cast<C_00833908*>(self->field_0008);
    if (!sceneObject)
        return 0;

    void* attachments = reinterpret_cast<void*>(sceneObject->vGetField016c());
    int attachmentIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, attachments, 0)[0x98 / 4])(
                attachments, nullptr, attachmentId);

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* effectDefinitions = CSF_AT(void*, manager, 0x1FCC);
    const char* effectName = (&g_psz_PFX_IMPACTO_BALA)[effectType];
    int effectIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, effectDefinitions, 0)[0x0C / 4])(
                effectDefinitions, nullptr, effectName);

    if (attachmentIndex == -1 || effectIndex == -1)
        return 0;

    manager = reinterpret_cast<void*>(GetValue8B0554());
    effectDefinitions = CSF_AT(void*, manager, 0x1FCC);
    void* effectDefinition =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, effectDefinitions, 0)[0x08 / 4])(
                effectDefinitions, nullptr, effectIndex);

    sceneObject = reinterpret_cast<C_00833908*>(self->field_0008);
    attachments = reinterpret_cast<void*>(sceneObject->vGetField016c());
    void* attachment =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, attachments, 0)[0x8C / 4])(
                attachments, nullptr, attachmentIndex);

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
    void* effect =
        reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, registry, 0)[0x14 / 4])(
                registry, nullptr,
                reinterpret_cast<std::uint8_t*>(effectDefinition) + 4);

    if (effect)
    {
        void** effectVtable = CSF_AT(void**, effect, 0);
        void* attachmentTransform =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, attachment, 0)[0x18 / 4])(
                    attachment, nullptr);

        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            effectVtable[0x44 / 4])(effect, nullptr, attachmentTransform);

        if (resetEffect)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, effect, 0)[0x28 / 4])(effect, nullptr, 0);
        }
    }

    return reinterpret_cast<int>(effect);
}

HOOK(0x004064E0, C_0082d70c_CreateImpactEffect, "auto");
