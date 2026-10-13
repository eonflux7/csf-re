#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int*, __fastcall, ProfileSavegames_GetScenarioCollection, 0x00515CE0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuProfiles_SyncListboxScrollToSelection, 0x0053BBD0, (CMenuProfiles* self, void*));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x00714750, (CGuiListBoxBase* self, void* , int itemHeight));
CSF_FUNC(int, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x00722DF0, (void* self, void* edx));

static int __fastcall CMenuProfiles_InitializeControls(
    CMenuProfiles* self, void* /*edx*/, uint32_t* context, int /*unused*/)
{
    self->vResetState();

    void* profile = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x8));
    self->field_002c = reinterpret_cast<uint32_t>(context);

    MenuProgressInput_UpdateGlobalState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    void* screen = reinterpret_cast<void*>(self->field_0024);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    screen = reinterpret_cast<void*>(self->field_0024);
    self->field_0018 = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x00842988, "BotonSelect"));

    if (self->field_0018)
    {
        int* scenarios = ProfileSavegames_GetScenarioCollection(profile, nullptr);
        uint32_t begin = CSF_AT(uint32_t, scenarios, 0x4);
        int count = begin
            ? static_cast<int32_t>(CSF_AT(uint32_t, scenarios, 0x8) - begin) / 80
            : 0;

        // The original replaces only the low byte of the count.
        int enabled = (count & ~0xFF) | (count != 0);
        void* button = reinterpret_cast<void*>(self->field_0018);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, button, 0)[0x68 / 4])(button, nullptr, enabled, 1, 1);
    }

    screen = reinterpret_cast<void*>(self->field_0024);
    uint32_t createButton =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xFC / 4])(
                screen, nullptr, CSF_STR(0x0084297C, "BotonCreate"));
    screen = reinterpret_cast<void*>(self->field_0024);
    self->field_001c = createButton;

    self->field_0020 =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xFC / 4])(
                screen, nullptr, CSF_STR(0x00842970, "BotonDelete"));

    if (self->field_0020)
    {
        int* scenarios = ProfileSavegames_GetScenarioCollection(profile, nullptr);
        uint32_t begin = CSF_AT(uint32_t, scenarios, 0x4);
        int count = begin
            ? static_cast<int32_t>(CSF_AT(uint32_t, scenarios, 0x8) - begin) / 80
            : 0;

        int enabled = (count & ~0xFF) | (count != 0);
        void* button = reinterpret_cast<void*>(self->field_0020);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, button, 0)[0x68 / 4])(button, nullptr, enabled, 1, 1);
    }

    screen = reinterpret_cast<void*>(self->field_0024);
    self->field_0014 =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xFC / 4])(
                screen, nullptr, CSF_STR(0x00842960, "ListboxProfiles"));

    if (self->field_0014)
    {
        void* storedObject = reinterpret_cast<void*>(
            ForwardToStoredObjectVirtualSlot40(
                reinterpret_cast<void*>(self->field_0014), nullptr));
        if (storedObject)
        {
            int itemHeight = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, storedObject, 0)[0x18 / 4])(storedObject, nullptr, 1);
            CGuiListBoxBase_SetItemHeight(
                reinterpret_cast<CGuiListBoxBase*>(self->field_0014),
                nullptr, itemHeight);
        }
    }

    self->vProcessActionAndRefresh(static_cast<int>(self->field_0024));
    CMenuProfiles_SyncListboxScrollToSelection(self, nullptr);

    screen = reinterpret_cast<void*>(self->field_0024);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, screen, 0)[0xA8 / 4])(screen, nullptr, 0);

    return static_cast<int>(self->field_0024);
}

HOOK(0x0053BDD0, CMenuProfiles_InitializeControls, "auto");
