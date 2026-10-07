#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Virtual slot at vtable byte offset 0x28 (index 10) of the renderer text object
// stored at CGuiListBox::m_pRowText. Returns a pointer to an inner object, or null.
using RowTextQueryFn = void* (__fastcall*)(void* self, void* /*edx*/);

// Virtual slot at vtable byte offset 0x18 (index 6) of the inner object returned
// by RowTextQueryFn; takes one stack int argument.
using InnerSetFn = int (__fastcall*)(void* self, void* /*edx*/, int arg);

// Returns 20 (0x14) when the list box has no obtainable row-text inner object,
// otherwise asks that inner object for the value selected by argument 1 and
// returns its result.
static int __fastcall CGuiListBox_GetRowTextHeight(CGuiListBox* self, void* /*edx*/)
{
    void* pRowText = reinterpret_cast<void*>(self->m_pRowText);
    void** pRowTextVtbl = *reinterpret_cast<void***>(pRowText);
    void* pInner = reinterpret_cast<RowTextQueryFn>(pRowTextVtbl[0x28 / 4])(pRowText, nullptr);
    if (!pInner)
        return 20;

    // Original reloads m_pRowText and repeats the query after the first call.
    void* pRowTextAgain = reinterpret_cast<void*>(self->m_pRowText);
    void** pRowTextVtblAgain = *reinterpret_cast<void***>(pRowTextAgain);
    void* pInnerAgain = reinterpret_cast<RowTextQueryFn>(pRowTextVtblAgain[0x28 / 4])(pRowTextAgain, nullptr);

    void** pInnerVtbl = *reinterpret_cast<void***>(pInnerAgain);
    return reinterpret_cast<InnerSetFn>(pInnerVtbl[0x18 / 4])(pInnerAgain, nullptr, 1);
}

HOOK(0x722CB0, CGuiListBox_GetRowTextHeight, "auto");
