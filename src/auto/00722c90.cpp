// CGuiListBox (vtable 0x00865070) — small helper on the LISTBOX row text object.
//
// Original (0x722C90):
//   mov ecx, [ecx + 0x218]   ; this->m_pRowText (renderer text object, slot +0x58)
//   mov eax, [ecx]           ; its vtable
//   call [eax + 0x28]        ; virtual slot +0x28 (no args)
//   neg eax / sbb eax,eax / neg eax   ; eax = (result != 0)
//   ret
#include <cstdint>

#include "types/auto_classes.h"
#include "hook.h"

// Returns nonzero when the renderer text object held in m_pRowText reports a
// nonzero result from its virtual slot +0x28. The original only tests the
// returned value for zero, so the exact meaning of the slot stays opaque here.
static int __fastcall CGuiListBox_IsRowTextVisible(CGuiListBox* self, void* /*edx*/)
{
    void* pRowText = reinterpret_cast<void*>(self->m_pRowText);
    void** vtbl = *reinterpret_cast<void***>(pRowText);

    typedef int (__fastcall* RowTextSlotFn)(void* self, void* edx);
    RowTextSlotFn slot = reinterpret_cast<RowTextSlotFn>(vtbl[0x28 / 4]);

    int result = slot(pRowText, nullptr);
    return result != 0 ? 1 : 0;
}

HOOK(0x722C90, CGuiListBox_IsRowTextVisible, "auto");
