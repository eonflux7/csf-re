#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, MenuMpStats_ForwardToLayout541230, 0x00541570, (int pList, int value));
CSF_FUNC(int*, __cdecl, MenuSaveGame_RebuildListContainer, 0x00541650, (S_005139c0* container));
CSF_FUNC(int, __fastcall, CMenuMpOnline_ScrollViewToTarget, 0x00542CD0, (CMenuMpOnline* self, void*));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x00714750, (CGuiListBoxBase* self, void* , int itemHeight));
CSF_FUNC(int, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x00722DF0, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);

static int __fastcall CMenuLoadGame_InitializeScreen(
    CMenuLoadGame* self, void* /*edx*/, int context, int /*unused*/)
{
    if (!((bool(__fastcall*)(void*, void*))
              CSF_AT(void**, self, 0)[0x30 / 4])(self, nullptr)
        && !self->field_0014)
    {
        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        ((void(__fastcall*)(void*, void*, CMenuLoadGame*))
             CSF_AT(void**, globalState, 0)[0x18 / 4])(
            globalState, nullptr, self);
    }

    self->vResetStateAndForwardToVirtual();
    self->field_0018 = context;

    void* progressState = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    MenuProgressInput_UpdateGlobalState(progressState, nullptr);

    void* layout = reinterpret_cast<void*>(self->field_0014);
    ((void(__fastcall*)(void*, void*))
         CSF_AT(void**, layout, 0)[0x16C / 4])(layout, nullptr);

    self->vSetBackButton(static_cast<int>(self->field_0014));

    layout = reinterpret_cast<void*>(self->field_0014);
    CGuiListBoxBase* loadGameList =
        ((CGuiListBoxBase*(__fastcall*)(void*, void*, const char*))
             CSF_AT(void**, layout, 0)[0xFC / 4])(
            layout, nullptr, CSF_STR(0x00842208, "ListboxLoadGames"));
    self->field_0100 = reinterpret_cast<uint32_t>(loadGameList);

    if (loadGameList)
    {
        void* listObject = reinterpret_cast<void*>(
            ForwardToStoredObjectVirtualSlot40(loadGameList, nullptr));
        if (listObject)
        {
            int itemHeight =
                ((int(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, listObject, 0)[0x18 / 4])(
                    listObject, nullptr, 1);
            CGuiListBoxBase_SetItemHeight(
                reinterpret_cast<CGuiListBoxBase*>(self->field_0100),
                nullptr, itemHeight);
        }

        auto* container = reinterpret_cast<S_005139c0*>(
            reinterpret_cast<uint8_t*>(self) + 0x108);
        MenuSaveGame_RebuildListContainer(container);
        MenuMpStats_ForwardToLayout541230(
            reinterpret_cast<int>(container),
            static_cast<int>(self->field_0100));
    }

    self->vInitBotonLoad();
    CMenuMpOnline_ScrollViewToTarget(
        reinterpret_cast<CMenuMpOnline*>(self), nullptr);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    const char* tacticName = g_pszMenu_P_Tactic;
    void* resourceManager = CSF_AT(void*, globalState, 0x14);
    int tactic =
        ((int(__fastcall*)(void*, void*, const char*))
             CSF_AT(void**, resourceManager, 0)[0x5C / 4])(
            resourceManager, nullptr, tacticName);

    void** layoutVtable =
        CSF_AT(void**, reinterpret_cast<void*>(self->field_0014), 0);
    int resource =
        ((int(__fastcall*)(void*, void*, int, int))
             CSF_AT(void**, resourceManager, 0)[0x74 / 4])(
            resourceManager, nullptr, tactic, 0);

    ((void(__fastcall*)(void*, void*, int))layoutVtable[0x1D0 / 4])(
        reinterpret_cast<void*>(self->field_0014), nullptr, resource);

    return static_cast<int>(self->field_0014);
}

HOOK(0x00539830, CMenuLoadGame_InitializeScreen, "auto");
