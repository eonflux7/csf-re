#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, MenuMpStats_ForwardToLayout541230, 0x00541570, (int pList, int value));
CSF_FUNC(int*, __cdecl, MenuSaveGame_RebuildListContainer, 0x00541650, (S_005139c0* container));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ScrollToTrackedItem, 0x005449D0, (CMenuSaveGame* self, void*));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x00714750, (CGuiListBoxBase* self, void* , int itemHeight));
CSF_FUNC(void*, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x00722DF0, (void* self, void* edx));

static uint32_t __fastcall CMenuSaveGame_InitializeControls(
    CMenuSaveGame* self, void* /*edx*/, uint32_t context, int /*unused*/)
{
    self->field_0050 = -1;
    self->field_0054 = -1;
    self->field_004c = 0;
    self->field_001c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0040 = context;

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    MenuProgressInput_UpdateGlobalState(globalState, nullptr);

    void* layout = reinterpret_cast<void*>(self->field_0044);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, layout, 0)[0x16c / 4])(layout, nullptr);

    layout = reinterpret_cast<void*>(self->field_0044);
    void* newButton = ((void*(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, layout, 0)[0xfc / 4])(
            layout, nullptr, CSF_STR(0x00843B18, "BotonNew"));
    self->field_0010 = reinterpret_cast<uint32_t>(newButton);

    layout = reinterpret_cast<void*>(self->field_0044);
    void* eraseButton = ((void*(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, layout, 0)[0xfc / 4])(
            layout, nullptr, CSF_STR(0x00843B0C, "BotonErase"));
    self->field_0014 = reinterpret_cast<uint32_t>(eraseButton);
    if (eraseButton)
    {
        ((void(__fastcall*)(void*, void*, int, int, int))
            CSF_AT(void**, eraseButton, 0)[0x68 / 4])(
                eraseButton, nullptr, 1, 1, 1);
    }

    layout = reinterpret_cast<void*>(self->field_0044);
    void* backButton = ((void*(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, layout, 0)[0xfc / 4])(
            layout, nullptr, CSF_STR(0x00843B00, "BotonBack"));
    self->field_0018 = reinterpret_cast<uint32_t>(backButton);

    layout = reinterpret_cast<void*>(self->field_0044);
    void* saveGamesList = ((void*(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, layout, 0)[0xfc / 4])(
            layout, nullptr, CSF_STR(0x00843AEC, "ListboxSaveGames"));
    self->field_001c = reinterpret_cast<uint32_t>(saveGamesList);

    if (saveGamesList)
    {
        void* storedObject = ForwardToStoredObjectVirtualSlot40(saveGamesList, nullptr);
        if (storedObject)
        {
            int itemHeight = ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, storedObject, 0)[0x18 / 4])(
                    storedObject, nullptr, 1);
            CGuiListBoxBase_SetItemHeight(
                reinterpret_cast<CGuiListBoxBase*>(self->field_001c),
                nullptr, itemHeight);
        }

        auto* container = reinterpret_cast<S_005139c0*>(
            reinterpret_cast<uint8_t*>(self) + 0x20);
        MenuSaveGame_RebuildListContainer(container);
        MenuMpStats_ForwardToLayout541230(
            reinterpret_cast<int>(container),
            static_cast<int>(self->field_001c));
    }

    CMenuSaveGame_ScrollToTrackedItem(self, nullptr);
    return self->field_0044;
}

HOOK(0x00544AA0, CMenuSaveGame_InitializeControls, "auto");
