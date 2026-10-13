#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, C_0082d410_CopyStringFrom, 0x00410B20, (C_0082d410* self, void* , C_0082d410* source));
CSF_FUNC(int, __fastcall, CampaignEntries_GetEntryCount, 0x0043E070, (void* self, void*));
CSF_FUNC(int, __stdcall, CMenuScenarioSelect_LookupValueByName, 0x0052D910, (CMenuScenarioSelect* a1));
CSF_FUNC(int, __fastcall, CMenuScenarioSelect_UpdateSelectedScenarioStars, 0x0052DB50, (CMenuScenarioSelect* self, void*));
CSF_FUNC(void, __fastcall, CMenuScenarioSelect_UpdateScenarioForKey, 0x0052DE50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __cdecl, CoreParser_ResolveEntryByKey, 0x00692440, (unsigned int key, int flags));
CSF_FUNC(int, __fastcall, CGuiListBox_GetItemCount_714860, 0x00714860, (CGuiListBox* self, void*));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0, (void* self, void* edx, int entry));

static int __fastcall CMenuScenarioSelect_RefreshState(
    CMenuScenarioSelect* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0064);
    if (!result)
        return result;

    auto* listBox = reinterpret_cast<CGuiListBox*>(self->field_0054);
    if (!listBox)
        return result;

    result = static_cast<int>(self->field_006c);
    if (!result)
        return result;

    result = CGuiListBox_GetItemCount_714860(listBox, nullptr);
    if (result <= 0)
    {
        self->field_003c = -1;
        return result;
    }

    result = GetStoredValueAtIndex(
        reinterpret_cast<void*>(self->field_0054), nullptr, 0);
    if (result == -1)
    {
        self->field_003c = -1;
        return result;
    }

    const std::uint32_t selectedIndex =
        static_cast<std::uint32_t>(GetIndexedTableValue_714C20(
            reinterpret_cast<int*>(self->field_0054), nullptr, result, 0));

    result = CampaignEntries_GetEntryCount(
        reinterpret_cast<std::uint8_t*>(self) + 0x44, nullptr);
    if (selectedIndex >= static_cast<std::uint32_t>(result))
    {
        self->field_003c = -1;
        return result;
    }

    if (self->field_003c == selectedIndex)
        return result;

    auto* entries = reinterpret_cast<std::uint8_t*>(self->field_0048);
    self->field_003c = selectedIndex;
    auto* selectedEntry = entries + 196 * selectedIndex;
    const int firstKey = CSF_AT(int, selectedEntry, 0x3C);

    alignas(8) std::uint8_t nameBytes[0x1C];
    C_0082d410& name = *reinterpret_cast<C_0082d410*>(nameBytes);
    C_0082d410_CopyStringFrom(
        &name, nullptr, reinterpret_cast<C_0082d410*>(selectedEntry + 64));

    // The supplied binding types this stack string argument as a menu pointer.
    int nameValue = CMenuScenarioSelect_LookupValueByName(
        reinterpret_cast<CMenuScenarioSelect*>(&name));
    int resolvedEntry = CoreParser_ResolveEntryByKey(firstKey, nameValue);
    C_00720DF0_ForwardCallToChildVtblSlot16(
        reinterpret_cast<void*>(self->field_0064), nullptr, resolvedEntry);

    const std::uint32_t entryOffset = 196 * self->field_003c;
    entries = reinterpret_cast<std::uint8_t*>(self->field_0048);
    const int secondKey = CSF_AT(int, entries + entryOffset, 0x5C);
    AssignStringSubstring(&name, nullptr, entries + entryOffset + 96, 0, -1);

    nameValue = CMenuScenarioSelect_LookupValueByName(
        reinterpret_cast<CMenuScenarioSelect*>(&name));
    resolvedEntry = CoreParser_ResolveEntryByKey(secondKey, nameValue);
    C_00720DF0_ForwardCallToChildVtblSlot16(
        reinterpret_cast<void*>(self->field_006c), nullptr, resolvedEntry);

    CMenuScenarioSelect_UpdateScenarioForKey(self, nullptr, secondKey);
    CMenuScenarioSelect_UpdateSelectedScenarioStars(self, nullptr);
    return CMenuSaveGame_ResetStringStorage(&name, nullptr);
}

HOOK(0x0052DF20, CMenuScenarioSelect_RefreshState, "auto");
