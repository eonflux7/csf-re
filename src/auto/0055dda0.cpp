#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00833908_SetOwnedContext, 0x0055D860, (void* self, void* edx, void* a2));
CSF_FUNC(int16_t, __cdecl, CoreVariantValues_CopyWideString, 0x00695680, (char* dest, char* src));
CSF_FUNC(int, __fastcall, CGuiListBox_GetItemCount_714860, 0x00714860, (CGuiListBox* self, void*));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, GetCellStringAddress, 0x00723030, (void* self, void* , int firstIndex, int secondIndex));

CSF_GLOBAL(const char*, g_pszMenu_P_LbFIOp, 0x00899B68);

static int __fastcall CMenuPlayerList_HandlePlayerSelection(
    CMenuPlayerList* self, void* /*edx*/, void* event)
{
    auto* list = reinterpret_cast<CGuiListBox*>(self->field_0024);
    if (list && list == CSF_AT(CGuiListBox*, event, 0x04))
    {
        if (CGuiListBox_GetItemCount_714860(list, nullptr) > 0)
        {
            const int row = GetStoredValueAtIndex(
                reinterpret_cast<void*>(self->field_0024), nullptr, 0);
            const int cellString = GetCellStringAddress(
                reinterpret_cast<void*>(self->field_0024), nullptr, row, 0);

            void* menu = reinterpret_cast<void*>(self->field_0014);
            ((void(__fastcall*)(void*, void*, CMenuPlayerList*, int))
                CSF_AT(void**, menu, 0)[0x60 / 4])(
                    menu, nullptr, self, 0);

            menu = reinterpret_cast<void*>(self->field_0014);
            void* playerInfo =
                ((void*(__fastcall*)(void*, void*, const char*))
                    CSF_AT(void**, menu, 0)[0x5C / 4])(
                        menu, nullptr, g_pszMenu_P_LbFIOp);

            char* context = reinterpret_cast<char*>(self) + 0x30;
            CoreVariantValues_CopyWideString(
                context, reinterpret_cast<char*>(cellString));

            self->field_0230 = GetIndexedTableValue_714C20(
                reinterpret_cast<int*>(self->field_0024), nullptr, row, 1);
            self->field_0234 = GetIndexedTableValue_714C20(
                reinterpret_cast<int*>(self->field_0024), nullptr, row, 0);

            C_00833908_SetOwnedContext(playerInfo, nullptr, context);

            menu = reinterpret_cast<void*>(self->field_0014);
            ((void(__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, menu, 0)[0x58 / 4])(
                    menu, nullptr, g_pszMenu_P_LbFIOp);
        }
    }
    else
    {
        const uint32_t resetControl = self->field_0028;
        if (resetControl && resetControl == CSF_AT(uint32_t, event, 0x04))
            self->vResetAndShowDefaultLabel();
    }

    return ((int(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x48 / 4])(self, nullptr, event);
}

HOOK(0x0055DDA0, CMenuPlayerList_HandlePlayerSelection, "auto");
