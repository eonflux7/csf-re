#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Cheats_RefreshSelection, 0x0042AF90, (int group, int context, CGuiListBoxBase* listBox));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpOnline_ScrollViewToTarget, 0x00542CD0, (CMenuMpOnline* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x00714750, (CGuiListBoxBase* self, void* , int itemHeight));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (CGuiListBoxBase* self, void* , int index));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (CGuiSlider* self, void*));
CSF_FUNC(int, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x00722DF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static int __fastcall CMenuControls_Initialize(
    CMenuControls* self, void* /*edx*/, int owner, int /*unused*/)
{
    if (!self->field_0014 &&
        !reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            self->vtbl[0x30 / 4])(self, nullptr))
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        reinterpret_cast<void(__fastcall*)(void*, void*, CMenuControls*)>(
            CSF_AT(void**, state, 0)[0x18 / 4])(state, nullptr, self);
    }

    self->field_02d4 = -1;
    self->field_02d8 = -1;
    self->field_02cc = -1;
    self->field_02d0 = -1;
    self->field_02dc = -1;
    self->field_0018 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->vClearField0010();

    self->field_0018 = owner;
    CMenuMpOnline_ScrollViewToTarget(
        reinterpret_cast<CMenuMpOnline*>(self), nullptr);
    void* inputState = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    MenuProgressInput_UpdateGlobalState(inputState, nullptr);

    void* menu = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, menu, 0)[0x16C / 4])(menu, nullptr);

    menu = reinterpret_cast<void*>(self->field_0014);
    uint32_t control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, menu, 0)[0xFC / 4])(
            menu, nullptr, CSF_STR(0x00843690, "ListboxConfiguracion"));
    self->field_002c = control;
    if (control)
    {
        void* storedObject = reinterpret_cast<void*>(
            ForwardToStoredObjectVirtualSlot40(
                reinterpret_cast<void*>(control), nullptr));
        if (storedObject)
        {
            int itemHeight = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, storedObject, 0)[0x18 / 4])(
                    storedObject, nullptr, 1);
            CGuiListBoxBase_SetItemHeight(
                reinterpret_cast<CGuiListBoxBase*>(self->field_002c),
                nullptr, itemHeight);
        }
    }

    menu = reinterpret_cast<void*>(self->field_0014);
    control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, menu, 0)[0xFC / 4])(
            menu, nullptr, CSF_STR(0x00843680, "BotonMovement"));
    self->field_0030 = control;
    if (control)
        CGuiSlider_ForwardSelfToParent(
            reinterpret_cast<CGuiSlider*>(control), nullptr);

    menu = reinterpret_cast<void*>(self->field_0014);
    control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, menu, 0)[0xFC / 4])(
            menu, nullptr, CSF_STR(0x00843674, "BotonLook"));
    menu = reinterpret_cast<void*>(self->field_0014);
    self->field_0034 = control;

    control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, menu, 0)[0xFC / 4])(
            menu, nullptr, CSF_STR(0x00843668, "BotonAttack"));
    menu = reinterpret_cast<void*>(self->field_0014);
    self->field_0038 = control;

    control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, menu, 0)[0xFC / 4])(
            menu, nullptr, CSF_STR(0x0084365C, "BotonOther"));
    menu = reinterpret_cast<void*>(self->field_0014);
    self->field_003c = control;

    control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, menu, 0)[0xFC / 4])(
            menu, nullptr, CSF_STR(0x00843650, "BotonReset"));
    menu = reinterpret_cast<void*>(self->field_0014);
    self->field_0040 = control;

    control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, menu, 0)[0xFC / 4])(
            menu, nullptr, CSF_STR(0x008424E4, "BotonSave"));
    self->field_0044 = control;

    // The string's leading word and its storage occupy one 0x1C-byte stack object.
    alignas(8) uint8_t textBytes[0x1C];
    if (control &&
        !CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xCC))
    {
        void* saveButton = reinterpret_cast<void*>(self->field_0044);
        void* label = reinterpret_cast<void*>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, saveButton, 0)[0x110 / 4])(
                    saveButton, nullptr));

        CSF_AT(uint32_t, textBytes, 0x18) = 7;
        CSF_AT(uint32_t, textBytes, 0x14) = 0;
        CSF_AT(uint16_t, textBytes, 0x4) = 0;
        InitStringFromLiteral(textBytes, CSF_STR(0x008427FC, "GE03"), 1);

        void* text = CSF_AT(void*, textBytes, 0x4);
        if (CSF_AT(uint32_t, textBytes, 0x18) < 8)
            text = textBytes + 0x4;
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, label, 0)[0x40 / 4])(label, nullptr, text);

        if (CSF_AT(uint32_t, textBytes, 0x18) >= 8 &&
            CSF_AT(void*, textBytes, 0x4))
        {
            Mem_FreeDebug(CSF_AT(void*, textBytes, 0x4));
        }
    }

    self->vSetBackButton(static_cast<int>(self->field_0014));
    bool hasList = self->field_002c != 0;
    self->field_02c8 = 0;
    if (hasList)
    {
        void* settings = reinterpret_cast<void*>(self->field_0020);
        if (settings)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*)>(
                CSF_AT(void**, settings, 0)[0x18 / 4])(
                    settings, nullptr, reinterpret_cast<uint8_t*>(self) + 0x48);
        }
        Cheats_RefreshSelection(
            static_cast<int>(self->field_02c8),
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x48),
            reinterpret_cast<CGuiListBoxBase*>(self->field_002c));
        CGuiListBoxBase_AddSelectedItem(
            reinterpret_cast<CGuiListBoxBase*>(self->field_002c), nullptr, 0);
    }

    CGuiListBoxBase* list = reinterpret_cast<CGuiListBoxBase*>(self->field_002c);
    if (list)
    {
        void* header = reinterpret_cast<void*>(list->vGetField180());
        if (header)
        {
            CSF_AT(uint32_t, textBytes, 0x18) = 7;
            CSF_AT(uint32_t, textBytes, 0x14) = 0;
            CSF_AT(uint16_t, textBytes, 0x4) = 0;
            void* initializedText = InitStringFromLiteral(
                textBytes, CSF_STR(0x00843648, "c026"), 1);
            void* text;
            if (CSF_AT(uint32_t, initializedText, 0x18) < 8)
                text = reinterpret_cast<uint8_t*>(initializedText) + 0x4;
            else
                text = CSF_AT(void*, initializedText, 0x4);
            reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, header, 0)[0x40 / 4])(header, nullptr, text);

            if (CSF_AT(uint32_t, textBytes, 0x18) >= 8 &&
                CSF_AT(void*, textBytes, 0x4))
            {
                Mem_FreeDebug(CSF_AT(void*, textBytes, 0x4));
            }
        }
    }

    void* ownerObject = reinterpret_cast<void*>(self->field_0018);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, ownerObject, 0)[0x9C / 4])(ownerObject, nullptr, 0);
    ownerObject = reinterpret_cast<void*>(self->field_0018);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, ownerObject, 0)[0xA8 / 4])(ownerObject, nullptr);
    return static_cast<int>(self->field_0014);
}

HOOK(0x00543210, CMenuControls_Initialize, "auto");
