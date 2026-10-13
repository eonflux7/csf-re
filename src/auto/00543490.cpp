#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(void, __cdecl, Cheats_RefreshSelection, 0x0042AF90, (int group, int context, CGuiListBoxBase* listBox));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __fastcall, MenuControls_DispatchTail, 0x00542C40, (int* self, void*));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(int, __fastcall, CGuiListBox_GetItemCount_714860, 0x00714860, (CGuiListBox* self, void*));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (CGuiListBoxBase* self, void* , int index));

static int __fastcall CMenuControls_DispatchCommand(
    CMenuControls* self, void* /*edx*/, int command)
{
    void* window = reinterpret_cast<void*>(self->field_0014);
    int result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, window, 0)[0x104 / 4])(window, nullptr);

    if (!result || self->field_02dc != static_cast<uint32_t>(-1))
        return result;

    auto* list = reinterpret_cast<CGuiListBoxBase*>(self->field_002c);
    void* header = list ? reinterpret_cast<void*>(list->vGetField180()) : nullptr;
    void* event = reinterpret_cast<void*>(command);

    int group = -1;
    const char* title = nullptr;
    uint32_t control = self->field_0030;
    if (control && control == CSF_AT(uint32_t, event, 0x4))
    {
        group = 0;
        title = CSF_STR(0x00843648, "c026");
    }
    else
    {
        control = self->field_0034;
        if (control && control == CSF_AT(uint32_t, event, 0x4))
        {
            group = 1;
            title = CSF_STR(0x008436C0, "c027");
        }
        else
        {
            control = self->field_0038;
            if (control && control == CSF_AT(uint32_t, event, 0x4))
            {
                group = 2;
                title = CSF_STR(0x008436B8, "c028");
            }
            else
            {
                control = self->field_003c;
                if (control && control == CSF_AT(uint32_t, event, 0x4))
                {
                    group = 3;
                    title = CSF_STR(0x008436B0, "c029");
                }
            }
        }
    }

    alignas(8) uint8_t localizedStringBytes[0x1C];

    if (group != -1)
    {
        CSF_AT(uint32_t, localizedStringBytes, 0x18) = 7;
        CSF_AT(uint32_t, localizedStringBytes, 0x14) = 0;
        CSF_AT(uint16_t, localizedStringBytes, 0x4) = 0;
        void* localizedString =
            InitStringFromLiteral(localizedStringBytes, title, 1);
        const char* text =
            CSF_AT(uint32_t, localizedString, 0x18) < 8u
                ? reinterpret_cast<const char*>(localizedString) + 4
                : CSF_AT(const char*, localizedString, 0x4);

        reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, header, 0)[0x40 / 4])(header, nullptr, text);

        Cheats_RefreshSelection(
            group,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x48),
            reinterpret_cast<CGuiListBoxBase*>(self->field_002c));

        list = reinterpret_cast<CGuiListBoxBase*>(self->field_002c);
        self->field_02c8 = group;
        if (list)
            CGuiListBoxBase_AddSelectedItem(list, nullptr, 0);
        return SaveGame_DestroyLocalizedString(localizedStringBytes, nullptr);
    }

    control = self->field_0040;
    if (control && control == CSF_AT(uint32_t, event, 0x4))
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        result = GameBddLoader_DispatchMenuRequest(
            state, nullptr, 1129263153, 1, 3, 0);
        self->field_02dc = 4;
        return result;
    }

    control = self->field_0044;
    if (control && control == CSF_AT(uint32_t, event, 0x4))
    {
        MenuControls_DispatchTail(reinterpret_cast<int*>(self), nullptr);
        return self->vUpdateOptionText();
    }

    auto* controlsList = reinterpret_cast<CGuiListBox*>(self->field_002c);
    if (!controlsList ||
        reinterpret_cast<uint32_t>(controlsList) !=
            CSF_AT(uint32_t, event, 0x4))
    {
        return self->vHandleLinkedItemChange(command);
    }

    result = CGuiListBox_GetItemCount_714860(controlsList, nullptr);
    if (result <= 0)
        return result;

    result = GetStoredValueAtIndex(
        reinterpret_cast<void*>(self->field_002c), nullptr, 0);
    const int row = result;
    if (row == -1)
        return result;

    self->field_02d8 = row;
    self->field_02d4 = GetIndexedTableValue_714C20(
        reinterpret_cast<int*>(self->field_002c), nullptr, row, 1);

    CSF_AT(uint32_t, localizedStringBytes, 0x18) = 7;
    CSF_AT(uint32_t, localizedStringBytes, 0x14) = 0;
    CSF_AT(uint16_t, localizedStringBytes, 0x4) = 0;
    void* localizedString = InitStringFromLiteral(
        localizedStringBytes, CSF_STR(0x008436A8, "c000"), 1);
    const char* text =
        CSF_AT(uint32_t, localizedString, 0x18) < 8u
            ? reinterpret_cast<const char*>(localizedString) + 4
            : CSF_AT(const char*, localizedString, 0x4);

    list = reinterpret_cast<CGuiListBoxBase*>(self->field_002c);
    reinterpret_cast<void(__fastcall*)(
        void*, void*, int, int, const char*, uint32_t)>(
        CSF_AT(void**, list, 0)[0x294 / 4])(
            list, nullptr, row, 1, text, self->field_02d4);

    self->field_02dc = 0;
    return SaveGame_DestroyLocalizedString(localizedStringBytes, nullptr);
}

HOOK(0x00543490, CMenuControls_DispatchCommand, "auto");
