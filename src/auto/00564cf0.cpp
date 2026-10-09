#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));

CSF_GLOBAL(int, g_SelectedAccountIndex, 0x89A5E8);
CSF_GLOBAL(uint32_t, g_dwAccountEditScreenState, 0x89A5EC);

static int __fastcall MenuAccounts_OpenSelectedAccountForEditing(void* self, void* /*edx*/)
{
    GetGlobalValue_8B056C();
    void* accountManager = CSF_AT(
        void*, reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x14);
    void** accountVtable = CSF_AT(void**, accountManager, 0);

    int rowIndex = GetStoredValueAtIndex(
        CSF_AT(void*, self, 0x14), nullptr, 0);
    int accountIndex = GetIndexedTableValue_714C20(
        CSF_AT(int*, self, 0x14), nullptr, rowIndex, 0);

    int result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        accountVtable[0x30 / 4])(accountManager, nullptr, accountIndex);

    if (result)
    {
        rowIndex = GetStoredValueAtIndex(
            CSF_AT(void*, self, 0x14), nullptr, 0);
        result = GetIndexedTableValue_714C20(
            CSF_AT(int*, self, 0x14), nullptr, rowIndex, 0);
        g_SelectedAccountIndex = result;
        g_dwAccountEditScreenState = 1;
    }

    return result;
}

HOOK(0x00564CF0, MenuAccounts_OpenSelectedAccountForEditing, "auto");
