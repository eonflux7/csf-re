#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuAccounts_ScrollToTargetDelta, 0x00564E80, (CMenuAccounts* self, void*));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x00714750, (CGuiListBoxBase* self, void* , int itemHeight));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660, (CGuiListBoxBase* self, void*));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (CGuiListBoxBase* self, void* , int index));
CSF_FUNC(void*, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x00722DF0, (void* self, void* edx));

static int __fastcall CMenuAccounts_InitializeControls(
    CMenuAccounts* self, void* /*edx*/, int context, int /*unused*/)
{
    if (!self->field_003c)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        reinterpret_cast<void (__fastcall*)(void*, void*, CMenuAccounts*)>(
            CSF_AT(void**, state, 0)[0x18 / 4])(state, nullptr, self);
    }

    reinterpret_cast<void (__fastcall*)(CMenuAccounts*, void*)>(
        reinterpret_cast<void**>(self->vtbl)[0x5c / 4])(self, nullptr);

    self->field_0038 = context;
    MenuProgressInput_UpdateGlobalState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    void* screen = reinterpret_cast<void*>(self->field_003c);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x16c / 4])(screen, nullptr);

    screen = reinterpret_cast<void*>(self->field_003c);
    uint32_t control = reinterpret_cast<uint32_t (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, screen, 0)[0xfc / 4])(
            screen, nullptr, CSF_STR(0x00845B84, "BotonCrear"));
    screen = reinterpret_cast<void*>(self->field_003c);
    self->field_0018 = control;

    control = reinterpret_cast<uint32_t (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, screen, 0)[0xfc / 4])(
            screen, nullptr, CSF_STR(0x00845B70, "BotonSeleccionar"));
    screen = reinterpret_cast<void*>(self->field_003c);
    self->field_0020 = control;

    self->field_0024 =
        reinterpret_cast<uint32_t (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xfc / 4])(
                screen, nullptr, CSF_STR(0x00845B64, "BotonBorrar"));

    void* object = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

    object = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

    if (self->field_0014)
    {
        void* storedObject = ForwardToStoredObjectVirtualSlot40(
            reinterpret_cast<void*>(self->field_0014), nullptr);
        if (storedObject)
        {
            int itemHeight = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, storedObject, 0)[0x18 / 4])(storedObject, nullptr, 1);
            CGuiListBoxBase_SetItemHeight(
                reinterpret_cast<CGuiListBoxBase*>(self->field_0014),
                nullptr, itemHeight);
        }

        CGuiListBoxBase_ClearStorageAndSendEvent(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0014), nullptr);
        CGuiListBoxBase_AddSelectedItem(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0014), nullptr, 0);
    }

    reinterpret_cast<void (__fastcall*)(CMenuAccounts*, void*)>(
        reinterpret_cast<void**>(self->vtbl)[0x50 / 4])(self, nullptr);

    int* screenArgument = reinterpret_cast<int*>(self->field_003c);
    auto handleScreen = reinterpret_cast<void (__fastcall*)(CMenuAccounts*, void*, int*)>(
        reinterpret_cast<void**>(self->vtbl)[0x44 / 4]);
    self->field_0030 = 1;
    handleScreen(self, nullptr, screenArgument);

    CMenuAccounts_ScrollToTargetDelta(self, nullptr);
    return static_cast<int>(self->field_003c);
}

HOOK(0x00564FF0, CMenuAccounts_InitializeControls, "auto");
