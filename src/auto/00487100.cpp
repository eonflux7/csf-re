#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __stdcall, CharActionFsm_ApplyItemTransform, 0x004850B0, (void* a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, FindRecordIndexById, 0x00485980, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, CharActionFsm_ApplyIndex, 0x00486CC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneObject_GetTransform, 0x00493890, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static char __fastcall CCharActionFsm_SelectItemFromHandle(
    CCharActionFsm* self, void* /*edx*/, int handle)
{
    void* sceneObject = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!sceneObject)
        return 0;

    void* services = reinterpret_cast<void*>(GetValue8B0554());
    void* itemDatabase = CSF_AT(void*, services, 0x1FC0);
    void* itemDefinition =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, itemDatabase, 0)[0x8 / 4])(
                itemDatabase, nullptr, CSF_AT(int, sceneObject, 0x184));

    int itemIndex = self->vFindFirstMatchingHandleIndex(CSF_AT(int, itemDefinition, 0xDC), 8);
    if (itemIndex == -1)
        return static_cast<char>(itemIndex);

    bool wasSelected = self->field_0024 == static_cast<uint32_t>(itemIndex);
    uint32_t transform[4]{};

    auto* owner = reinterpret_cast<CCharWeaponFsm*>(self->field_0008);
    if (owner->field_0054 == 1)
    {
        void* item = reinterpret_cast<void**>(self->field_0018)[itemIndex];
        int itemType =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, item, 0)[0x4C / 4])(item, nullptr);

        if (itemType == 10)
        {
            void* provider = GetCachedObjectProvider(item, nullptr);
            int providerIndex = FindRecordIndexById(
                self, nullptr,
                static_cast<int>(reinterpret_cast<std::uintptr_t>(provider)));

            if (providerIndex != -1)
            {
                void** providerSlot =
                    reinterpret_cast<void**>(self->field_0078) + providerIndex;
                auto* sourceTransform = reinterpret_cast<uint32_t*>(
                    SceneObject_GetTransform(
                        sceneObject, nullptr,
                        static_cast<int>(reinterpret_cast<std::uintptr_t>(transform))));
                CharActionFsm_ApplyItemTransform(*providerSlot, sourceTransform);

                if (CSF_AT(uint8_t, transform, 0x4))
                    RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(transform[0]));

                CharActionFsm_ApplyIndex(self, nullptr, providerIndex);
            }
        }
    }

    void** itemSlot = reinterpret_cast<void**>(self->field_0018) + itemIndex;
    auto* sourceTransform = reinterpret_cast<uint32_t*>(
        SceneObject_GetTransform(
            sceneObject, nullptr,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(transform))));
    CharActionFsm_ApplyItemTransform(*itemSlot, sourceTransform);

    if (CSF_AT(uint8_t, transform, 0x4))
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(transform[0]));

    self->vRemoveSlotAndShift(itemIndex);

    void* controller = CSF_AT(void*, sceneObject, 0x34);
    int sceneItemId = CSF_AT(int, sceneObject, 0x184);
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, controller, 0)[0xC8 / 4])(
            controller, nullptr, sceneItemId, 1);

    if (!wasSelected)
        return 0;

    int actionIndex = self->vFindProviderIndex(CSF_AT(int, sceneObject, 0x184));
    if (actionIndex == -1)
        return static_cast<char>(actionIndex);

    return static_cast<char>(self->vSetCurrentWeaponAction(actionIndex));
}

HOOK(0x00487100, CCharActionFsm_SelectItemFromHandle, "auto");
