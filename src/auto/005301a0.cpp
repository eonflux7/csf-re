#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(bool, __fastcall, BddLoader_IsRecordListEmpty, 0x0050E990, (uint8_t* self, void*));
CSF_FUNC(int, __fastcall, ProfilesSaveGames_ResetBuffers, 0x00514AC0, (int* self, void*));
CSF_FUNC(int, __fastcall, C_00840af8_ComputeChildHandleFromSlot, 0x00515BD0, (C_00840af8* self, void*));
CSF_FUNC(int*, __fastcall, ProfileSavegames_GetScenarioCollection, 0x00515CE0, (void* self, void*));
CSF_FUNC(int, __fastcall, S_004f21d0_InitFromSource, 0x005167D0, (S_004f21d0* self, void* , void* source));
CSF_FUNC(int, __cdecl, GetValue_8B0574, 0x00518980, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuSceneSelect_AlignChildToReferencedPosition, 0x0052FBD0, (CMenuSceneSelect* self, void*));
CSF_FUNC(void*, __fastcall, CMenuSceneSelect_PopulateSceneList, 0x0052FFD0, (CMenuSceneSelect* self, void*));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x00714750, (CGuiListBoxBase* self, void* , int itemHeight));
CSF_FUNC(int, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x00722DF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static uint32_t __fastcall CMenuSceneSelect_InitializeSceneList(
    CMenuSceneSelect* self, void* /*edx*/, uint32_t selectionContext, int /*unused*/)
{
    if (!self->field_0014)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        reinterpret_cast<void(__fastcall*)(void*, void*, CMenuSceneSelect*)>(
            CSF_AT(void**, state, 0)[0x18 / 4])(state, nullptr, self);
    }

    self->field_0034 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    AssignStringRange_429B60(
        reinterpret_cast<uint8_t*>(self) + 0x18, nullptr,
        reinterpret_cast<char*>(&g_byte_82E65C), 0);

    self->vClearField0010();
    self->field_0034 = selectionContext;
    GetValue_8B0574();
    CMenuSceneSelect_AlignChildToReferencedPosition(self, nullptr);
    MenuProgressInput_UpdateGlobalState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    void* screen = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    screen = reinterpret_cast<void*>(self->field_0014);
    auto* sceneList = reinterpret_cast<CGuiListBoxBase*>(
        reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xFC / 4])(
                screen, nullptr, CSF_STR(0x008419E8, "ListboxScenes")));
    self->field_0050 = reinterpret_cast<uint32_t>(sceneList);
    if (!sceneList)
        return 0;

    void* heightProvider = reinterpret_cast<void*>(
        ForwardToStoredObjectVirtualSlot40(sceneList, nullptr));
    if (heightProvider)
    {
        int itemHeight = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, heightProvider, 0)[0x18 / 4])(
                heightProvider, nullptr, 1);
        CGuiListBoxBase_SetItemHeight(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0050),
            nullptr, itemHeight);
    }

    self->vSetBackButton(static_cast<int>(self->field_0014));
    CMenuSceneSelect_PopulateSceneList(self, nullptr);

    void* session = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (CSF_AT(uint8_t, session, 0xA6C))
    {
        void* profiles = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        auto* profile = CSF_AT(C_00840af8*, profiles, 0x08);
        if (profile)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                profile->vtbl[0x34 / 4])(profile, nullptr);

            int* scenarios = ProfileSavegames_GetScenarioCollection(profile, nullptr);
            if (BddLoader_IsRecordListEmpty(
                    reinterpret_cast<uint8_t*>(scenarios), nullptr))
            {
                alignas(8) uint8_t patternBytes[0x1C];
                FormatterSetPattern(
                    patternBytes, nullptr, CSF_STR(0x008419E0, "Phantom"));
                profile->vSlot54(reinterpret_cast<unsigned int*>(patternBytes));
                SaveGame_DestroyLocalizedString(patternBytes, nullptr);
            }
            else if (C_00840af8_ComputeChildHandleFromSlot(profile, nullptr) == -1)
            {
                void* firstEntry = reinterpret_cast<void*>(profile->vSlot1C(0));
                alignas(8) uint8_t entryBytes[0x50];
                S_004f21d0_InitFromSource(
                    reinterpret_cast<S_004f21d0*>(entryBytes), nullptr, firstEntry);
                profile->vSlot58(reinterpret_cast<int>(entryBytes));
                ProfilesSaveGames_ResetBuffers(
                    reinterpret_cast<int*>(entryBytes), nullptr);
            }
        }
    }

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* stateController = CSF_AT(void*, state, 0x10);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, stateController, 0)[0x08 / 4])(stateController, nullptr);
    return self->field_0014;
}

HOOK(0x005301A0, CMenuSceneSelect_InitializeSceneList, "auto");
