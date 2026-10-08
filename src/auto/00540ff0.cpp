#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall callee: int CGuiListBox::GetItemCount(void)
CSF_FUNC(int, __fastcall, ListBox_GetItemCount, 0x00714860, (void* self, void* edx));

// __thiscall callee: int GetStoredValueAtIndex(int index), pops 4
CSF_FUNC(int, __fastcall, StoredValue_GetAtIndex, 0x00714CA0, (void* self, void* edx, int index));

// __thiscall callee: int GetIndexedTableValue(int value, int extra), pops 8
CSF_FUNC(int, __fastcall, IndexedTable_GetValue, 0x00714C20, (void* self, void* edx, int value, int extra));

static void* ListBoxPointerFromInt(int listBox)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(listBox)));
}

// __cdecl: returns 1 and writes the indexed table value of the first stored
// entry to *outValue when the list box is non-null, has items and the stored
// index is valid; otherwise returns 0 and leaves *outValue untouched.
static char __cdecl ReadFirstStoredIndexedValue(int listBox, int* outValue)
{
    if (listBox == 0)
        return 0;

    void* listBoxObj = ListBoxPointerFromInt(listBox);

    if (ListBox_GetItemCount(listBoxObj, nullptr) <= 0)
        return 0;

    int storedIndex = StoredValue_GetAtIndex(listBoxObj, nullptr, 0);
    if (storedIndex == -1)
        return 0;

    *outValue = IndexedTable_GetValue(listBoxObj, nullptr, storedIndex, 0);
    return 1;
}

HOOK(0x00540FF0, ReadFirstStoredIndexedValue, "auto");
